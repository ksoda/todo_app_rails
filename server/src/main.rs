#[macro_use]
extern crate diesel;
#[macro_use]
extern crate log;

use std::{env, io};

use actix_files as fs;
use actix_session::CookieSession;
use actix_web::middleware::{errhandlers::ErrorHandlers, Logger};
use actix_web::{http, web, App, HttpServer};
use dotenv::dotenv;
use tera::Tera;

mod api;
mod db;
mod model;
mod schema;
mod session;

static SESSION_SIGNING_KEY: &[u8] = &[0; 32];

#[actix_rt::main]
async fn main() -> io::Result<()> {
    dotenv().ok();

    env::set_var("RUST_LOG", "actix_todo=debug,actix_web=info");
    env_logger::init();

    let database_url = env::var("DATABASE_URL").expect("DATABASE_URL must be set");
    let pool = db::init_pool(&database_url).expect("Failed to create pool");

    let app = move || {
        debug!("Constructing the App");

        let templates: Tera = Tera::new("templates/**/*").unwrap();

        let session_store = CookieSession::signed(SESSION_SIGNING_KEY).secure(false);

        let error_handlers = ErrorHandlers::new()
            .handler(
                http::StatusCode::INTERNAL_SERVER_ERROR,
                api::internal_server_error,
            )
            .handler(http::StatusCode::BAD_REQUEST, api::bad_request)
            .handler(http::StatusCode::NOT_FOUND, api::not_found);

        App::new()
            .data(templates)
            .data(pool.clone())
            .wrap(Logger::default())
            .wrap(session_store)
            .wrap(error_handlers)
            .service(web::resource("/").route(web::get().to(api::index)))
            .service(web::resource("/todo").route(web::post().to(api::create)))
            .service(web::resource("/todo/{id}").route(web::post().to(api::update)))
            .service(fs::Files::new("/static", "static/"))
    };

    let port = env::var("PORT")
        .unwrap_or_else(|_| "3000".to_string())
        .parse::<String>()
        .expect("PORT must be a number string");

    debug!("Starting server on {}", port);
    HttpServer::new(app)
        .bind(format!("0.0.0.0:{}", port))?
        .run()
        .await
}