(lang dune 2.0)
(name luv-cli)
(source (github idkjs/luv-cli))
(license MIT)
(authors "Alain Armand")
(maintainers "Alain Armand")

(generate_opam_files true)

(package
 (name luv-cli)
 (synopsis "luv wrapped in a cli")
 (description "\
luv wrapped in a cli
")
 (depends
  (ocaml (>= 4.06.0))
  (dune (>= 2.0))
  ; Add Rely when released on Opam
  ; (rely :with-test)
  (odoc :with-doc)
  (reason :build)
  base
  stdio))
