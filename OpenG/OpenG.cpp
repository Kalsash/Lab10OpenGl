//https://github.com/Kalsash/Lab10OpenGl

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// используется для преобразования координат
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";
//Фрагментные или пиксельные шейдеры модифицируют цвет отображаемых пикселей
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
)";

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    GLuint vertexShader, fragmentShader, shaderProgram;
    GLint success;
    GLchar infoLog[512];

    // Создание и компиляция вершинного шейдера
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//Объекты шейдеров создаются функцией glCreateShader.
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);//Инициализируются исходным кодом функцией glShaderSource
    glCompileShader(vertexShader);// Компилируется функцией glCompileShader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //Обработать ошибки
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "Error compiling vertex shader:\n" << infoLog << std::endl;
    }

    // Создание и компиляция фрагментного шейдера
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Error compiling fragment shader:\n" << infoLog << std::endl;
    }

    // Создание шейдерной программы
    shaderProgram = glCreateProgram(); // Создаём шейдерную программу: glCreateProgram
    glAttachShader(shaderProgram, vertexShader);// Присоединяем каждый шейдер функцией glAttachShader
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);// Линкуем программу glLinkProgram
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // Обработать ошибки
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Error linking shader program:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Определение вершин треугольника
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    //VAO используется для определения атрибутов вершин и их расположения, а 
    //VBO используется для хранения самих вершинных данных.
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);//Создаём VBO: glGenBuffers

    // Привязка VAO
    glBindVertexArray(VAO);// Вызываем демонов и биндим VBO

    // Заполнение VBO данными
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Биндим VBO: glBindBuffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Заполняем его данными функцией glBufferData

    // Настройка атрибутов вершинного массива
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Сбрасывает текущий связанный вершинный массив, 
    //возвращая OpenGL к состоянию, когда ни один вершинный массив не связан
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Использование шейдерной программы
        glUseProgram(shaderProgram);// Установить шейдерную программу текущей: glUseProgram

        // Привязка VAO
        glBindVertexArray(VAO); // Вызываем демонов и биндим VBO

        // Отрисовка треугольника
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Сбрасывает текущий связанный вершинный массив, 
     //возвращая OpenGL к состоянию, когда ни один вершинный массив не связан
        glBindVertexArray(0);

       // чтобы обновлять содержимое окна и обрабатывать пользовательский ввод.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO); //Удаление VBO: glDeleteBuffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram); //Удаление шейдерной программы: glDeleteProgram

    glfwTerminate();
    return 0;
}