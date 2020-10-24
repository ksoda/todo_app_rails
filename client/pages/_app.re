/* [%raw
    import { ApolloProvider } from "@apollo/client";
    import { useApollo } from "../src/apolloClient";

    export default function App({ Component, pageProps }) {
      const apolloClient = useApollo(pageProps.initialApolloState);

      return (
        <ApolloProvider client={apolloClient}>
          <Component {...pageProps} />
        </ApolloProvider>
      );
    }
   ] */

%raw
"import '../styles/main.css'";

// This type is based on the getInitialProps return value.
// If you are using getServerSideProps or getStaticProps, you probably
// will never need this
// See https://nextjs.org/docs/advanced-features/custom-app
type pageProps = {.};

module PageComponent = {
  type t = React.component(pageProps);
};

type props = {
  [@bs.as "Component"]
  component: PageComponent.t,
  pageProps,
};

// We are not using `[@react.component]` since we will never

// use <App/> within our Reason code. It's only used within `pages/_app.js`
let default = (props: props): React.element => {
  let {component, pageProps} = props;

  let router = Next.Router.useRouter();

  let content = React.createElement(component, pageProps);

  <ReasonApollo.Provider client=Client.instance>
    <ApolloHooks.Provider client=Client.instance>
      {switch (router.route) {
       | "/examples" =>
         <MainLayout>
           <h1 className="font-bold"> {React.string("Examples Section")} </h1>
           <div> content </div>
         </MainLayout>
       | _ => <MainLayout> content </MainLayout>
       }}
    </ApolloHooks.Provider>
  </ReasonApollo.Provider>;
};
