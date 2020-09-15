module Link = {
  [@bs.module "next/link"] [@react.component]
  external make:
    (
      ~href: string=?,
      ~_as: string=?,
      ~prefetch: option(bool)=?,
      ~replace: option(bool)=?,
      ~shallow: option(bool)=?,
      ~passHref: option(bool)=?,
      ~children: React.element
    ) =>
    React.element =
    "default";
};

module Head = {
  [@bs.module "next/head"] [@react.component]
  external make: (~children: React.element) => React.element = "default";
};

module Error = {
  [@bs.module "next/head"] [@react.component]
  external make: (~statusCode: int, ~children: React.element) => React.element =
    "default";
};

module Router = {
  [@bs.module "next/router"]
  external useRouter: unit => Js.t('a) = "useRouter";
};

module Config = {
  [@bs.module "next/config"] external getConfig: unit => 'a = "default";
};
module Dynamic = {
  [@bs.module "next/dynamic"] external dynamic: unit => 'a = "default";
};