Windows, MSVC, x64. Соответственно проект настроен на это.

Запуск либо через решение/проект Visual Studio,
либо с помощью .exe в одной директории с папками Shaders и Models (и может быть Textures)
Скомпилированный .exe уже есть в папке bin
assimp-vc142-mt.dll и zlib.dll должны находиться в одной папке с .exe или в одной папке с решением/проектом.

Зависимости проекта: см. Dependencies (glew, GLFW, glm, SOIL, Assimp). В прикрепленном к github архиве то, чем пользуюсь я.
Стоит заметить, что Assimp не поддерживает 32bit.

Поддержка форматов: в теории то, что поддерживает Assimp. 
На практике:
obj + mtl + текстуры (тоже не все форматы, например progressive .jpg не поддерживается, см. SOIL) - точно да. 
Точно нет:
.blend , см. github Assimp -> issues (github.com/assimp/assimp/issues/3316)
.c4d - no suitable reader

Textures в данный содержит текстуры для "ручного", прямого OpenGL использования. Не-ручное - это через Assimp.

Управление: мышь, колесо мыши настраивает FOV, WASD и L_CTRL/SPACE для перемещения. ESC чтобы закрыть окно
F - Wireframe рендеринг