let luv = () =>  {
  Luv.DNS.getaddrinfo(
    ~family=`INET,
    ~node="google.com",
    ~service="80",
    (),
    (result) => {
      let address = List.hd(Result.get_ok(result)).addr;
      let socket = Luv.TCP.init() |> Result.get_ok;
      Luv.TCP.connect(
        socket,
        address,
        (_) => {
          Luv.Stream.write(
            socket,
            [Luv.Buffer.from_string("GET / HTTP/1.1\r\n\r\n")],
            (_, _) => Luv.Stream.shutdown(socket, ignore)
          );
          Luv.Stream.read_start(
            socket,
            fun
            | Error(`EOF) => Luv.Handle.close(socket, ignore)
            | Error(_) => exit(2)
            | Ok(response) => print_string(Luv.Buffer.to_string(response))
          );
        }
      );
    }
  );
  ignore(Luv.Loop.run(): bool);
};