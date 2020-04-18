# luv-cli

[![Actions Status](https://github.com/idkjs/luv-cli/workflows/CI/badge.svg)](https://github.com/idkjs/luv-cli/actions)

luv wrapped in a cli

## Scaffold with Spin

Running `spin new native luv-cli` creates a new directory from where ever you are in the file system.

I answered `1` to keep this `reason` all the way through.

```sh
❯ pwd
~/Github
❯ spin new native luv-cli
📡  Updating official templates.
Done!

Project name: luv-cli
Project slug: [luv-cli]
Description: [A short, but powerful statement about your project] luv wrapped in a cli
Which syntax do you use?
1 - Reason
2 - OCaml
Choose from (1, 2): 1
Which package manager do you use?
1 - Esy
2 - Opam
Choose from (1, 2): [1] 1
Which test framework do you prefer?
1 - Rely
2 - Alcotest
Choose from (1, 2): [1] 1
Which CI/CD platform do you use
1 - Github
2 - None
Choose from (1, 2): [1] 1
> cd luv-cli
```

This will produce the following directory structure:

```
> tree -L 2
.
├── CHANGES.md
├── CONTRIBUTING.md
├── LICENSE
├── README.md
├── _esy
│   └── default
├── bin
│   ├── dune
│   └── luv_cli_app.re
├── dune-project
├── esy.json
├── esy.lock
│   ├── index.json
│   ├── opam
│   └── overrides
├── lib
│   ├── dune
│   ├── utils.re
│   └── utils.rei
├── luv-cli.opam
├── luv-cli.opam.template
├── node_modules
└── test
    ├── _snapshots
    ├── dune
    ├── support
    └── utils_test.re

11 directories, 16 files

```

## Test It

Run `esy start` or `esy x luv-cli.exe`

```sh
❯ esy start
Hello World!
```

## Adding Color

If you haven't noticed yet, I'm basically following [Micheal Kohl's](https://twitter.com/@citizen428) post [native-cli-apps-in-reasonml-part-1](https://citizen428.net/blog/native-cli-apps-in-reasonml-part-1/). To figure out how to add deps to a spin project. The `luv-with-esy` build using worked out ok but I can't figure out how to add it to a spin project with getting build errors. Since Kohl, is a contributor, I am figuring he will show us so let's see.

Let add the to dependencies we will need:

```sh
esy add @reason-native/console
esy add @reason-native/pastel
```

Next we have to add them to the Dune configuration file in `lib/dune`. In this file we’ll change the `libraries` stanza from

`(libraries base)`

to

`(libraries base console.lib pastel.lib)`

Note how this is diffent than when we used `pesy`. `pesy` asks us to add our new deps to `package.json` and rebuilds our `dune` file for us.

We can use our new tools in a new `Utils.hello` function. Update `utils.re` with:

```re
open Pastel;

let hello = () =>
  <Pastel bold=true color=Green>
    "Hello, "
    <Pastel italic=true color=Red> "World!" </Pastel>
    " 👋"
  </Pastel>;
```

We also have to had this function to `utils.rei` as you will have noticed if you tried to run `esy start`.

Another `pesy/spin` difference is that you can run your the project as soon as you make the changes. With `pesy` we had to run `esy pesy` then `esy` then our run command.

With `spin` you just run `esy start`. I noticed that it did not work the first time you run `esy start` but runs the second time without making any changes. Feels like the build system is lagging maybe? I have no idea.

Running `esy start` will now get you:

![hello.png](./hello.png)

```sh
~/Github/luv-cli
❯ esy start
Hello World!
Hello, World! 👋
```

## Adding a Test

`spin` generated a test file for us when we ran the spin cli.

```re
open Test_framework;
open Luv_cli;

/** Test suite for the Utils module. */

let test_hello_with_name = (name, {expect}) => {
  let greeting = Utils.greet(name);
  let expected = "Hello " ++ name ++ "!";
  expect.string(greeting).toEqual(expected);
};

describe("Utils", ({test, _}) => {
  test("can greet Tom", test_hello_with_name("Tom"));
  test("can greet John", test_hello_with_name("John"));
});

```

Let's add another one for our new function:

```re
open Luv_cli;

/** Test suite for the Utils module. */
describe("Utils", ({test, _}) =>
  test("Utils.hello() returns a greeting", ({expect}) => {
    expect.string(Utils.hello()).toMatch("👋")
  })
);
```

Running `esy test` gets us 3 passing tests including our `Utils.hello` test.

Again, all we had to do was run `esy test`. We didn't have to rebuild. I'm finding that super convenient.

Here is what we got:

```sh
~/Github/luv-cli
❯ esy test
Running 2 test suites
 PASS  Utils
 PASS  Utils
```

## Adding Argument Parsing for Practice

Run `esy add @opam/cmdliner` the add `cmdliner` to `lib/dune` like we did before.

Our `libraries` stanza now looks like this:

```
(libraries base console.lib pastel.lib cmdliner)
```

`stanza`, by the way, as used in poetry, is what dune calls each line defining some behavior in project build. [wikipedia](https://en.wikipedia.org/wiki/Stanza). Appropriate use of the word, I think.

So now that we have [`Commandliner`](https://erratique.ch/software/cmdliner) let's use it in `bin/luv_cli_app`.

Add the following to `bin/luv_cli_app`:

```re
open Cmdliner;

let cmd = {
  let doc = "Simple CLI for Luvers built in Reason";

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
```

Notice I have `cmd` calling `Utils.helloInput`. I wanted to keep the other functions as they are to compare so lets add another to `utils.re` and `utils.rei`.

Here we change the previous `Utils.hello` so that it takes a string, and rename it `Utils.helloInput`

```re
let helloInput = (string) =>
  <Pastel bold=true color=Green>
    "Hello, "
    <Pastel italic=true color=Red> string </Pastel>
    " 👋"
  </Pastel>;
```

Then add `let helloInput: string => string;` to `utils.rei`.

Run `esy start Luvers` where `Luvers` is just the string the command expects.

```sh
❯ esy start Luvers
Hello World!
Hello, World! 👋
Hello, Luvers 👋
```

What happened here? We used `Open Cmdliner` in `bin/luv_cli_app` but we didnt add it to `bin/dune`. We only added it to `lib/dune` and we did not even use it there. So I as a matter of structuring our project, we are including everything we need in our `lib` and calling any library functions we might want to use from our `lib`. Just for fun, take `cmdliner` out of `lib/dune`, add it to `bin/dune` then run `esy start`. You should get the same result. The way [Kohl](https://twitter.com/@citizen428) did it make sense though. It keeps control of everything in one place. After all, at this point, the only time we use `cmdliner` is when calling it with from `lib/utils.re`. This makes sense to me.






## Contributing

Take a look at our [Contributing Guide](CONTRIBUTING.md).
