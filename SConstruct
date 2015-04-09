import os
env = Environment(ENV = {'PATH' : os.environ['PATH']})
env.Append(CPPPATH = ['/usr/include/', './src/'])
env.Append(LIBPATH = ['/usr/lib/', 'lib/'])
env.Append(LIBS = ['GL', 'GLEW', 'm', 'glut', 'GLU'])
env.Append(CFLAGS = ['-Wall', '-Werror', '-pedantic', '-std=c11'])

#Set scons to output object files to the "build" directory.
env.VariantDir('build', 'src', duplicate=0)
sources = [Glob("build/*.c"), Glob("build/*/*.c")]

object_list = env.Object(source = sources)
env.Program(target="out/bin/coh", source=object_list)
