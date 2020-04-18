open Luv_cli;

/** Main entry point for our application. */

let () = Stdio.print_endline @@ Utils.greet("World");
let () = Stdio.print_endline @@ Utils.hello();

open Cmdliner;

let cmd = {
  let doc = "Simple CLI built in Reason";

  let who = {
    let doc = "Who do you want to greet";
    Arg.(
      required
      & pos(0, some(string), None)
      & info([], ~docv="WHO", ~doc)
    );
  };

  let run = who => {
    Console.log @@ Utils.helloInput(who);
  };

  Term.(const(run) $ who, info("luv-cli", ~doc));
};

let () = Term.exit @@ Term.eval(cmd);