Import("env")
import gzip

def post_program_action(source, target, env):
    print("Building ", source[0].get_path() + ".gz")
    with open(source[0].get_abspath(), 'rb') as orig_file:
        with gzip.open(source[0].get_abspath() + ".gz", 'wb') as zipped_file:
            zipped_file.writelines(orig_file)

env.AddPostAction("buildprog", post_program_action)