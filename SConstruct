import os
env = Environment(ENV = {'PATH' : os.environ['PATH']})
env.Append(CPPPATH = ['/usr/include/', 'include/', './src/debug/', './src/gl/', './src/glut/', './src/math/', './src/test/', './src/util/', './src/lib/', './src/render/'])
env.Append(LIBPATH = ['/usr/lib/', 'lib/'])
env.Append(LIBS = ['GL', 'GLEW', 'm', 'glut', 'GLU'])
env.Append(CFLAGS = ['-Wall', '-Werror', '-pedantic', '-std=c99'])
sources = [Glob("src/*.c"), Glob("src/*/*.c")]

object_list = env.Object(source = sources)
env.Program(target="bin/coh", source=object_list)
