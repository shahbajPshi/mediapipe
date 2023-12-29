def link_pshi_files(srcs):
    """Links already pshi downloaded file with the current directory.

    Args:
         srcs: the names of the pshi_file target, which is also the name of
         the Pshi file in external_pshi_files.bzl. For example, if `name` is Foo,
         `pshi_file` will create a link to the downloaded file
         "@com_pshi_Foo_tfile" to the current directory as
         "Foo.tflite".
    """

    for src in srcs:
        archive_name = "com_pshi_%s" % src.replace("/", "_").replace(".", "_")
        native.genrule(
            name = "%s_ln" % archive_name,
            srcs = ["@%s//file" % archive_name],
            outs = [src],
            output_to_bindir = 1,
            cmd = "ln $< $@",
        )
