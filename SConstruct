import os
env = Environment(ENV = {'PATH' : os.environ['PATH']})
env.Append(CPPPATH = ['/usr/include/', './src/'])
env.Append(LIBPATH = ['/usr/lib/', 'lib/', './out/lib/'])
env.Append(LIBS = ['GL', 'm', 'GLU', 'libglfw'])
env.Append(CFLAGS = ['-Wall', '-Werror', '-pedantic', '-std=c11', '-ggdb'])

#Set scons to output object files to the "build" directory.
env.VariantDir('build', 'src', duplicate=0)
sources = [Glob("build/*.c"), Glob("build/*/*.c")]

object_list = env.Object(source = sources)
env.Program(target="out/bin/coh", source=object_list)
