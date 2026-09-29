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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include"TestClearColor.h"
#include"TestMenu.h"
#include "Test2DTexture.h"
float step = 0.1f; // Khoảng cách di chuyển camera

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

    ///////////////////////////////////////////////////////////////////////
    // IMGUI SETUPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP!!!!!!!!!!!!!!!!!!!!!!!
    // ==========================================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();

    const char* glsl_version = "#version 330"; // Hoặc #version 130 tùy bản OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // ==========================================
    ///////////////////////////////////////////////////////////////////////////////
   

    float levels = 4.0f;
    Shader basic_shader("res/Shader/BasicShader.shader");
    basic_shader.Bind();
    basic_shader.SetUniform1f("levels", levels); 
    basic_shader.UnBind();

    Renderer renderer;
    renderer.EnableBlend();
    renderer.SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    renderer.SetBlendEquation(GL_FUNC_ADD);
    renderer.EnableDepthTest();

    // Mở rộng Near/Far lên [-2000.0, 2000.0] để khi xoay 3D trục Z không bị clip
    Mat4 ortho;
    ortho.Ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -2000.0f, 2000.0f);
    Mat4 Model;
    Model.Identity();
    Mat4 Camera;
    Camera.Identity();
    int last_cells = 0;
    /////////////

    test::TestMenu* test_menu = new test::TestMenu();

    test_menu->RegisterTest<test::TestClearColor>("Test_clear_color");
    test_menu->RegisterTest<test::Test2DTexture>("Test_2D_Texture");

    while (!glfwWindowShouldClose(window))
    {
       
        //////////////////////////////////////////////////////////////
        // ==========================================
        // IMGUI SETUPPPPPPPPPPPPPPP!!!!!!
        glfwPollEvents(); 
        // Thông báo cho ImGui tính toán delta time, input bàn phím/chuột của frame này
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ==========================================
        //////////////////////////////////////////////////////////////// 
        //glClearColor(color_clear.r, color_clear.g, color_clear.b, color_clear.a);
        renderer.Clear();
        test_menu->OnUpdate(0.0f);
        test_menu->OnRender();
        
        //////////////////////////////////////////////////////////////// 
        // ==========================================
        // LOGICCCCCCCCCCCCCCCCCCC
        
       



        HandleInput();
        Camera.Translate(cameraPos);
      

        // ==========================================
        ////////////////////////////////////////////////////////////////////////////
        // ==========================================
        // IMGUI LOGICCCCCCCCCCCCCCC
        {
            ImGui::Begin("Test");
            test_menu->OnImGuiRender();
           
            if (test_menu->GetCurrentTest()!= test_menu)
            { 
                test_menu->GetCurrentTest()->OnImGuiRender();
                ImGui::SameLine();
                if (ImGui::Button("<-"))
                {
                    test_menu->DeleteCurrentTest();
                    test_menu->SetCurrentTest(test_menu);
                }
            }
            ImGui::End();
        }
       
        ImGui::Render();

        // Thực sự vẽ ImGui đè lên trên Scene OpenGL vừa vẽ ở Bước 2
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // ==========================================
        /////////////////////////////////////////////////////////////////////////////
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}