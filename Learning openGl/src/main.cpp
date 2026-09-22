#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Render.h"
#include "VAO.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h" 
#include "Maths.h"
#include <chrono>
#include <thread>
#include <windows.h>

float step = 5.0f; // Khoảng cách di chuyển camera

// Dùng Vec3 để lưu vị trí Camera riêng biệt
Vec3 cameraPos = { 0.0f, 0.0f, 0.0f };

void HandleInput()
{
    // Bấm W S A D sẽ dịch chuyển vị trí camera
    if (GetAsyncKeyState('W') & 0x8000) cameraPos.m_y += step; // Camera đi lên
    if (GetAsyncKeyState('S') & 0x8000) cameraPos.m_y -= step; // Camera đi xuống
    if (GetAsyncKeyState('A') & 0x8000) cameraPos.m_x -= step; // Camera sang trái
    if (GetAsyncKeyState('D') & 0x8000) cameraPos.m_x += step; // Camera sang phải
}

static float currentAngleX = 0.0f;

// Hàm xoay vật thể TẠI TÂM (Local Rotation)
Mat4 GetModelMatrix(Vec3 centerPosition)
{
    currentAngleX += 0.02f;
    if (currentAngleX >= 6.2831853f) {
        currentAngleX -= 6.2831853f;
    }

    float c = cosf(currentAngleX);
    float s = sinf(currentAngleX);

    // 1. Tạo ma trận xoay quanh trục X
    Mat4 rotX;
    rotX.Identity();
    rotX.Right = Vec4{ 1.0f, 0.0f, 0.0f, 0.0f };
    rotX.Up = Vec4{ 0.0f,    c,    s, 0.0f };
    rotX.Forward = Vec4{ 0.0f,   -s,    c, 0.0f };
    rotX.Position = Vec4{ 0.0f, 0.0f, 0.0f, 1.0f };

    // 2. Dịch chuyển vật thể từ gốc (0,0) tới vị trí thực tế
    Mat4 trans;
    trans.Identity();
    trans.Position = Vec4{ centerPosition.m_x, centerPosition.m_y, centerPosition.m_z, 1.0f };

    // Thứ tự: Xoay tại chỗ trước -> Rồi mới dịch chuyển ra vị trí
    // (Lưu ý: Tùy lớp Maths của bạn, nếu nhân cột thì Model = Trans * Rot)
    Mat4 model;
    model.Identity();
    model.Right = rotX.Right;
    model.Up = rotX.Up;
    model.Forward = rotX.Forward;
    model.Position = Vec4{ centerPosition.m_x, centerPosition.m_y, centerPosition.m_z, 1.0f };

    return model;
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Lỗi khởi tạo GLEW!\n";
    }

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // Đưa tọa độ 2 khối về TÂM GỐC TỌA ĐỘ (0,0) để khi xoay không bị lệch vòng cung
    // Chiều rộng = 400, Chiều cao = 400
    Vertex vertices[8] = {
        // ---------------- KHỐI 1 (Local Space: tâm tại 0,0) ----------------
        { { -200.0f,  200.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { {  200.0f,  200.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { {  200.0f, -200.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { { -200.0f, -200.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },

        // ---------------- KHỐI 2 (Local Space: tâm tại 0,0) ----------------
        { { -200.0f,  200.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { {  200.0f,  200.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        { {  200.0f, -200.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { { -200.0f, -200.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }
    };

    unsigned int indices[12] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    VAO vao;
    VertexBuffer vb(vertices, sizeof(vertices));
    IndexBuffer ib(indices, 12);
    VertexBufferLayout layout;

    layout.AddLayout<float>(3, GL_FALSE); // Position
    layout.AddLayout<float>(4, GL_FALSE); // Color
    layout.AddLayout<float>(2, GL_FALSE); // UV
    vao.AddVertexBuffer(vb, ib, layout);

    float levels = 4.0f;

    Shader cube_1_shader("res/Shader/BasicShader.shader");
    Shader cube_2_shader("res/Shader/BasicShader.shader");
    Texture texture1("res/image/dog.png");
    Texture texture2("res/image/cat.png");

    texture1.Bind(0);
    texture2.Bind(1);

    cube_1_shader.Bind();
    cube_1_shader.SetUniform1i("uTexture", 0);
    cube_1_shader.SetUniform1f("levels", levels);
    cube_1_shader.UnBind();

    cube_2_shader.Bind();
    cube_2_shader.SetUniform1i("uTexture", 1);
    cube_2_shader.SetUniform1f("levels", levels); // Đã sửa lỗi gán nhầm
    cube_2_shader.UnBind();

    Renderer renderer;
    renderer.EnableBlend();
    renderer.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderer.SetBlendEquation(GL_FUNC_ADD);
    renderer.EnableDepthTest();

    // Mở rộng Near/Far lên [-2000.0, 2000.0] để khi xoay 3D trục Z không bị clip
    Mat4 ortho;
    ortho.Ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -2000.0f, 2000.0f);

    // Vị trí tâm của 2 khối trong World Space
    Vec3 cube1_Pos = { 400.0f, 540.0f, 0.0f };  // Khối bên trái
    Vec3 cube2_Pos = { 1520.0f, 540.0f, 0.0f }; // Khối bên phải

    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        HandleInput();

        // Ma trận View (Invert vị trí camera)
        Mat4 ViewMatrix;
        ViewMatrix.Identity();
        ViewMatrix.Position = Vec4{ -cameraPos.m_x, -cameraPos.m_y, -cameraPos.m_z, 1.0f };

        // Lấy ma trận Model đã xoay tại vị trí đặt vật thể
        Mat4 Model1 = GetModelMatrix(cube1_Pos);
        Mat4 Model2 = GetModelMatrix(cube2_Pos);

        // Tính MVP cho Khối 1
        Mat4 MVP1 = ortho * ViewMatrix * Model1;
        cube_1_shader.Bind();
        cube_1_shader.SetUniform4Mat("u_MVP", MVP1);
        renderer.Draw(vao, cube_1_shader, 6, (const void*)0);

        // Tính MVP cho Khối 2
        Mat4 MVP2 = ortho * ViewMatrix * Model2;
        cube_2_shader.Bind();
        cube_2_shader.SetUniform4Mat("u_MVP", MVP2);
        renderer.Draw(vao, cube_2_shader, 6, (const void*)(6 * sizeof(unsigned int)));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}