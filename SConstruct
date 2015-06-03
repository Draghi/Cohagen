import os
import sys

env = None

platform = sys.platform
if platform == "win32":
	env = Environment(tools = ['mingw'], CC = 'gcc', ENV = os.environ)
	env.Append(LIBS = ['glfw3', 'opengl32', 'gdi32', 'm'])
elif platform == "darwin":
	env = Environment(CC = 'gcc', ENV = {'PATH' : os.environ['PATH']})
	env.Append(LIBS = ['libglfw3', 'm'])
	#env.Append(CFLAGS = '-framework OpenGL')
else:
	env = Environment(CC = 'gcc', ENV = {'PATH' : os.environ['PATH']})
	env.Append(LIBS = ['libglfw', 'GL', 'm'])
	
env.Append(CPPPATH = './src/')
env.Append(LIBPATH = './out/lib/')
env.Append(CFLAGS = ['-Wall', '-pedantic', '-std=c11', '-ggdb'])

#Set scons to output object files to the "build" directory.
env.VariantDir('./build', './src', duplicate=0)
sources = [Glob("./build/*.c"), Glob("./build/*/*.c"), Glob("./build/*/*/*.c")]

object_list = env.Object(source = sources)

env.Program(target="./out/bin/coh", source=object_list)
