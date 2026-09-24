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
int tmp[1000][1000];

void CreatCells(int Cells, std::vector<Vertex>& vertexs, std::vector<unsigned int>& indices)
{
    if (Cells <= 0 || Cells >= 999) return;
    vertexs.clear();
    indices.clear();
    vertexs.reserve((Cells + 1) * (Cells + 1));
    indices.reserve(Cells * Cells * 6);

    float width = 1920.0f / Cells;
    float height = 1080.0f / Cells;
    //
    // Sửa 1: Lặp theo số nguyên (int) để tránh sai số float
    for (int i = 0; i <= Cells; i++)
    {
        float y = i * height;
        for (int j = 0; j <= Cells; j++)
        {
            float x = j * width;
            Vertex vertex = { {x, y, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
            vertexs.emplace_back(vertex);
        }
    }

    // Sửa 2: Lưu chỉ số cho đủ (Cells + 1) x (Cells + 1) đỉnh
    unsigned int Count = 0;
    for (int i = 0; i <= Cells; i++)
    {
        for (int j = 0; j <= Cells; j++)
        {
            tmp[i][j] = Count++;
        }
    }

    // Sửa 3: Duyệt đủ từ 0 đến Cells - 1 ô
    for (int i = 0; i < Cells; i++)
    {
        for (int j = 0; j < Cells; j++)
        {
            indices.emplace_back(tmp[i][j]);
            indices.emplace_back(tmp[i + 1][j]);
            indices.emplace_back(tmp[i + 1][j + 1]);

            indices.emplace_back(tmp[i][j]);
            indices.emplace_back(tmp[i + 1][j + 1]);
            indices.emplace_back(tmp[i][j + 1]);
        }
    }
}

// Sửa 4: Phân chia ô Đen/Trắng chuẩn caro theo tọa độ (row, col)
std::vector<unsigned int> GetBlackGrid(std::vector<unsigned int>& indices)
{
    std::vector<unsigned int> BlackIndices;
    BlackIndices.reserve(indices.size() / 2);

    int totalCells = indices.size() / 6;
    int cellsPerRow = static_cast<int>(std::sqrt(totalCells));

    for (int cellIdx = 0; cellIdx < totalCells; cellIdx++)
    {
        int row = cellIdx / cellsPerRow;
        int col = cellIdx % cellsPerRow;

        if ((row + col) % 2 == 0) // Ô đen
        {
            int baseIdx = cellIdx * 6;
            for (int k = 0; k < 6; k++)
                BlackIndices.push_back(indices[baseIdx + k]);
        }
    }
    return BlackIndices;
}

std::vector<unsigned int> GetWhiteGrid(std::vector<unsigned int>& indices)
{
    std::vector<unsigned int> WhiteIndices;
    WhiteIndices.reserve(indices.size() / 2);

    int totalCells = indices.size() / 6;
    int cellsPerRow = static_cast<int>(std::sqrt(totalCells));

    for (int cellIdx = 0; cellIdx < totalCells; cellIdx++)
    {
        int row = cellIdx / cellsPerRow;
        int col = cellIdx % cellsPerRow;

        if ((row + col) % 2 != 0) // Ô trắng
        {
            int baseIdx = cellIdx * 6;
            for (int k = 0; k < 6; k++)
                WhiteIndices.push_back(indices[baseIdx + k]);
        }
    }
    return WhiteIndices;
}
void UpdateVao(VAO& vao1, VAO& vao2, std::vector<Vertex> vertices, std::vector<unsigned int> indices, VertexBufferLayout& layout, std::vector<unsigned int>&Blackin, std::vector<unsigned int>&Whitein,int cells)
{
    CreatCells(cells, vertices, indices);
    Blackin = GetBlackGrid(indices);
    Whitein = GetWhiteGrid(indices);
    VertexBuffer vb(vertices.data(), vertices.size() * sizeof(Vertex));
    IndexBuffer ib1(Blackin.data(), Blackin.size());
    IndexBuffer ib2(Whitein.data(), Whitein.size());

    vao1.AddVertexBuffer(vb, ib1, layout);
    vao2.AddVertexBuffer(vb, ib2, layout);
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
    VertexBufferLayout layout;

    layout.AddLayout<float>(4, GL_FALSE); // Position
    layout.AddLayout<float>(4, GL_FALSE); // Color
    layout.AddLayout<float>(2, GL_FALSE); // UV
    
    VAO vao1;
    VAO vao2;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::vector<unsigned int> Blackin = GetBlackGrid(indices);
    std::vector<unsigned int> Whitein = GetWhiteGrid(indices);

    float levels = 4.0f;

    Shader basic_shader("res/Shader/BasicShader.shader");
   

    basic_shader.Bind();

    basic_shader.SetUniform1f("levels", levels); // Đã sửa lỗi gán nhầm
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
    while (!glfwWindowShouldClose(window))
    {
        static bool is_show_demo_window = false;
        static bool is_show_console = true;
        static Vec4 color_clear({ 0,0,0,0 });
        static Vec4 color_grid_black({ 0,0,0,1 });
        static Vec4 color_grid_white({ 1,1,1,1 });
        static Vec4 CoX = { 1,0,0,0 };
        static Vec4 CoY = { 0,1,0,0 };
        static Vec4 CoZ = { 0,0,1,0 };
        static Vec4 CoW = { 0,0,0,1 };
        static int cells=10;
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
        glClearColor(color_clear.r, color_clear.g, color_clear.b, color_clear.a);
        renderer.Clear();
        //////////////////////////////////////////////////////////////// 
        // ==========================================
        // LOGICCCCCCCCCCCCCCCCCCC
        if (cells != last_cells)
        {
            UpdateVao(vao1, vao2, vertices, indices, layout, Blackin, Whitein, cells);
            last_cells = cells;
        }
       



        HandleInput();
        Camera.Translate(cameraPos);
        basic_shader.Bind();
        Mat4 MVP = ortho * Camera * Model;
        basic_shader.SetUniform4Mat("u_MVP", MVP);
        basic_shader.SetUniform4f("uColor",color_grid_black);
        renderer.Draw(vao1, basic_shader, Blackin.size(),(const void*)0);
        basic_shader.SetUniform4f("uColor", color_grid_white);
        renderer.Draw(vao2, basic_shader, Whitein.size(), (const void*)0);

        // ==========================================
        ////////////////////////////////////////////////////////////////////////////
        // ==========================================
        // IMGUI LOGICCCCCCCCCCCCCCC
        
       
       

        if (is_show_demo_window)ImGui::ShowDemoWindow();

        {
            ImGui::Begin("Test2D");
           
            ImGui::SliderInt("Cells", &cells, 1, 1000);
            ImGui::ColorEdit4("color_grid_black", color_grid_black.elements);
            ImGui::ColorEdit4("color_grid_white", color_grid_white.elements);
           
            
            ImGui::DragFloat4("x", CoX.elements, 0.0001f);
            ImGui::DragFloat4("y", CoY.elements, 0.0001f);
            ImGui::DragFloat4("z", CoZ.elements, 0.0001f);
            ImGui::DragFloat4("w", CoW.elements, 0.0001f);
            Model.Columns[0] = { CoX.m_x,CoY.m_x,CoZ.m_x,CoW.m_x };
            Model.Columns[1] = { CoX.m_y,CoY.m_y,CoZ.m_y,CoW.m_y };
            Model.Columns[2] = { CoX.m_z,CoY.m_z,CoZ.m_z,CoW.m_z };
            Model.Columns[3] = { CoX.m_w,CoY.m_w,CoZ.m_w,CoW.m_w };
           

            ImGui::End();
        }
        
        if (ImGui::BeginMainMenuBar())
        {
             if (ImGui::BeginMenu("File"))
             {
                if (ImGui::MenuItem("Exit")) { glfwSetWindowShouldClose(window, true); }
                ImGui::EndMenu(); 
             }
             if (ImGui::BeginMenu("Windows"))
             {
                 ImGui::MenuItem("Show Demo Window", NULL, &is_show_demo_window);
                 ImGui::MenuItem("Show Console", NULL, &is_show_console);
                 ImGui::EndMenu();
             }
             if (ImGui::BeginMenu("Colors"))
             {
                 ImGui::ColorEdit4("color_clear", color_clear.elements);
                 ImGui::EndMenu();

             }
             ImGui::EndMainMenuBar();
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