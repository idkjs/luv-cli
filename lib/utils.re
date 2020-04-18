
open Pastel;
let greet = (name) => "Hello " ++ name ++ "!";

let hello = () =>
  <Pastel bold=true color=Green>
    "Hello, "
    <Pastel italic=true color=Red> "World!" </Pastel>
    " 👋"
  </Pastel>;

let helloInput = (string) =>
  <Pastel bold=true color=Green>
    "Hello, "
    <Pastel italic=true color=Red> string </Pastel>
    " 👋"
  </Pastel>;