#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<math.h>

#include"BezierCurve.h"
#include"Shader.h"
#include"imgui.h";
#include"imgui_impl_glfw.h";
#include"imgui_impl_opengl3.h";

#define Width 800
#define Height 600
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void EditCurve(GLFWwindow* window);
void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
void MousePosCallback(GLFWwindow* window, double xPos, double Ypos);
void ProcessInput(GLFWwindow* window);
void UpdateSubCurves();
//float controlPoints[9];
BezierCurve editorBezierCurve(30);
std::vector<BezierCurve> subCurves;
std::vector<float> curvePoints;
std::vector<float> handlePoints;
std::vector<float> points;
float t_value;
Point3f bezPoint;
bool bezPointCheck;
bool subDivideCheck;
bool subDivisionActive;
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(Width, Height, "BezierProject", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, MouseButtonCallBack);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetCursorPosCallback(window, MousePosCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    EditCurve(window);
    //UAEFlagCurves(window);
    //UAEFlag(window);

    glfwTerminate();

    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void EditCurve(GLFWwindow* window)
{
    Shader lineShader("Shaders/line.vert", "Shaders/line.frag");
    Shader pointShader("Shaders/point.vert", "Shaders/point.frag");
    // Set up vertex data (and buffer(s)) and attribute pointers
    auto pt1 = Point3f(-0.5f, -0.5f, 0.0f);
    auto pt2 = Point3f(-0.25f, 0.0f, 0.0f);

    editorBezierCurve.AddControlPoint(pt1);
    editorBezierCurve.AddControlPoint(pt2);
    editorBezierCurve.AddControlPoint(Point3f(0.5f, -0.5f, 0.0f));

    curvePoints = editorBezierCurve.DrawCurve();
    points = editorBezierCurve.GetControlPointArray();

    unsigned int VBO[6], VAO[6];
    
#pragma region Binding
    
    
    //single color curve and points 

    glGenVertexArrays(1,&VAO[0]);
    glGenBuffers(1, &VBO[0]);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(float), &curvePoints[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //clean
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //points
    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //clean
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //subcurve 1
    glGenVertexArrays(1, &VAO[2]);
    glGenBuffers(1, &VBO[2]);
    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, (curvePoints.size() / 2) * sizeof(float), &curvePoints[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //clean
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //subcurve 2
    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    glBindVertexArray(VAO[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, (curvePoints.size() / 2) * sizeof(float), &curvePoints[curvePoints.size() / 2], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //clean
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //points - curve1 
    glGenVertexArrays(1, &VAO[4]);
    glGenBuffers(1, &VBO[4]);
    glBindVertexArray(VAO[4]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, (points.size() / 2) * sizeof(float), &points[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //clean
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //points - curve2 
    glGenVertexArrays(1, &VAO[5]);
    glGenBuffers(1, &VBO[5]);
    glBindVertexArray(VAO[5]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, (points.size() / 2) * sizeof(float), &points[(points.size() / 2)], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //clean
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
   
#pragma endregion

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    float lineColor[4] = { 1.0f, 0.5f, 0.2f, 1.0f },
          pointColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float thickness = 5.0f;
    float pointSize = 10.0f;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input
        ProcessInput(window);
        if (bezPointCheck && !subDivisionActive)
        {
            cout << "enter t value " << endl;
            cin >> t_value;
            bezPoint = editorBezierCurve.BezPoint(t_value);
            cout << "bez point -" << bezPoint.X << " , " << bezPoint.Y << " , " << bezPoint.Z << endl;
            points.push_back(bezPoint.X);
            points.push_back(bezPoint.Y);
            points.push_back(bezPoint.Z);
        }
        if (subDivideCheck && !subDivisionActive)
        {
            t_value = NULL;
            cout << "enter t value , where you would like to divide the curve" << endl;
            cin >> t_value;
            subCurves = editorBezierCurve.DeCasteljauSubDivision(t_value);

            for (int i = 0; i < subCurves.size(); i++)
            {
                for (int j = 0; j < subCurves[i].controlPoints.size(); j++)
                {
                    subCurves[i].controlPoints[j].selected = false;
                }
            }

            editorBezierCurve.controlPoints.clear();
            UpdateSubCurves();
            subDivisionActive = true;
        }
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        lineShader.Use();
        glLineWidth(thickness);
        
        if (subDivisionActive)
        {
            lineShader.SetFloat4("lineColor", 1.0f, 0.5f, 0.2f, 1.0f);
            glBindVertexArray(VAO[2]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
            glBufferData(GL_ARRAY_BUFFER, (curvePoints.size() / 2) * sizeof(float), &curvePoints[0], GL_STATIC_DRAW);
            glDrawArrays(GL_LINE_STRIP, 0, (curvePoints.size() / 2)); // 2 vertices to draw a line

            lineShader.SetFloat4("lineColor", 0.5f, 1.0f, 0.0f, 1.0f);
            glBindVertexArray(VAO[2]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
            glBufferData(GL_ARRAY_BUFFER, (curvePoints.size() / 2) * sizeof(float), &curvePoints[(curvePoints.size() / 2)], GL_STATIC_DRAW);
            glDrawArrays(GL_LINE_STRIP, 0, (curvePoints.size() / 2)); // 2 vertices to draw a line

            pointShader.Use();
            glPointSize(10.0f);//set point size to 10 pixels
            pointShader.SetFloat4("pointColor", 1.0f, 0.5f, 0.2f, 1.0f);
            glBindVertexArray(VAO[4]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
            glBufferData(GL_ARRAY_BUFFER, (points.size()/2) * sizeof(float), &points[0], GL_STATIC_DRAW);
            glDrawArrays(GL_POINTS, 0, (points.size() / 2) / 3);

            pointShader.Use();
            glPointSize(10.0f);//set point size to 10 pixels
            pointShader.SetFloat4("pointColor", 0.5f, 1.0f, 0.0f, 1.0f);
            glBindVertexArray(VAO[5]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
            glBufferData(GL_ARRAY_BUFFER, (points.size() / 2) * sizeof(float), &points[(points.size() / 2)], GL_STATIC_DRAW);
            glDrawArrays(GL_POINTS, 0, (points.size() / 2) / 3);
        }
        else
        {
            lineShader.SetFloat4("lineColor", lineColor[0], lineColor[1], lineColor[2], lineColor[3]);
            glBindVertexArray(VAO[0]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
            glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(float), &curvePoints[0], GL_STATIC_DRAW);
            glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size() / 3); // 2 vertices to draw a line

            pointShader.Use();
            glPointSize(pointSize);//set point size to 10 pixels
            pointShader.SetFloat4("pointColor", pointColor[0], pointColor[1], pointColor[2], pointColor[3]);
            glBindVertexArray(VAO[1]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), &points[0], GL_STATIC_DRAW);
            glDrawArrays(GL_POINTS, 0, points.size() / 3);
        }

        ImGui::Begin("Curve Editor");
        ImGui::Text("this is a curve editor");
        ImGui::ColorEdit4("curveColor", lineColor);
        ImGui::ColorEdit4("pointColor", pointColor);
        ImGui::SliderFloat("thickness", &thickness, 3.0f, 10.f);
        ImGui::SliderFloat("pointSize", &pointSize, 8.0f, 15.f);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // De-allocate all resources once they've outlived their purpose
    auto size = subDivisionActive ? 6 : 2;
    for (int i = 0; i < size; i++)
    {
        glDeleteVertexArrays(1, &VAO[i]);
        glDeleteBuffers(1, &VBO[i]);
    }
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float worldXPos = xpos / (Width / 2) - 1;
        float worldYPos = -1 * (ypos / (Height / 2) - 1);

        if (!subDivisionActive)
        {
            //iterate through every control point
            for (int i = 0; i < editorBezierCurve.controlPoints.size(); i++)
            {
                auto minDistance = Point3f::Distance(editorBezierCurve.controlPoints[i], Point3f(worldXPos, worldYPos, 0.0f));
                if (minDistance < 0.1f)
                {
                    //std::cout << i << " - selected control point - " << editorBezierCurve.controlPoints[i].X << " ," << editorBezierCurve.controlPoints[i].Y << std::endl;
                    editorBezierCurve.controlPoints[i].selected = true;
                    return;
                }
            }
        }
        else
        {
            for (int i = 0; i < subCurves.size(); i++)
            {
                for (int j = 0; j < subCurves[i].controlPoints.size(); j++)
                {
                    auto minDistance = Point3f::Distance(subCurves[i].controlPoints[j], Point3f(worldXPos, worldYPos, 0.0f));
                    if (minDistance < 0.1f)
                    {
                        //std::cout << i << " - selected control point - " << editorBezierCurve.controlPoints[i].X << " ," << editorBezierCurve.controlPoints[i].Y << std::endl;
                        subCurves[i].controlPoints[j].selected = true;
                        return;
                    }
                }
            }
        }
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        //unselect all control point
        if (!subDivisionActive)
        {
            for (int i = 0; i < editorBezierCurve.controlPoints.size(); i++)
            {
                editorBezierCurve.controlPoints[i].selected = false;
            }
        }
        else
        {
            for (int i = 0; i < subCurves.size(); i++)
            {
                for (int j = 0; j < subCurves[i].controlPoints.size(); j++)
                {
                    subCurves[i].controlPoints[j].selected = false;
                }
            }
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        if (subDivisionActive)
            return;
        //add extra control points inbetween on right click
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float worldXPos = xpos / (Width / 2) - 1;
        float worldYPos = -1 * (ypos / (Height / 2) - 1);

        editorBezierCurve.controlPoints.insert(editorBezierCurve.controlPoints.end() - 1, Point3f(worldXPos, worldYPos, 0.0));
        curvePoints = editorBezierCurve.DrawCurve();
        points = editorBezierCurve.GetControlPointArray();
    }
}
void MousePosCallback(GLFWwindow* window, double xPos, double yPos)
{
    float worldXPos = xPos / (Width / 2) - 1;
    float worldYPos = -1 * (yPos / (Height / 2) - 1);

    if (!subDivisionActive)
    {
        for (int i = 0; i < editorBezierCurve.controlPoints.size(); i++)
        {
            if (editorBezierCurve.controlPoints[i].selected)
            {
                editorBezierCurve.controlPoints[i].X = worldXPos;
                editorBezierCurve.controlPoints[i].Y = worldYPos;

                curvePoints = editorBezierCurve.DrawCurve();
                points = editorBezierCurve.GetControlPointArray();

                return;
            }
        }
    }
    else
    {
        for (int i = 0; i < subCurves.size(); i++)
        {
            for (int j = 0; j < subCurves[i].controlPoints.size(); j++)
            {
                if (subCurves[i].controlPoints[j].selected)
                {
                    subCurves[i].controlPoints[j].X = worldXPos;
                    subCurves[i].controlPoints[j].Y = worldYPos;

                    UpdateSubCurves();
                }
            }
        }
    }
    
}
void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        bezPointCheck = true;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
        bezPointCheck = false;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        subDivideCheck = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        subDivideCheck = false;

}
void UpdateSubCurves()
{
    auto curve1 = subCurves[0].DrawCurve();
    curvePoints = curve1;
    auto curve2 = subCurves[1].DrawCurve();
    curvePoints.insert(curvePoints.end(), curve2.begin(), curve2.end());

    points = subCurves[0].GetControlPointArray();
    auto pts = subCurves[1].GetControlPointArray();
    points.insert(points.end(), pts.begin(), pts.end());

   
    /*for (int i = 0; i < subCurves[1].controlPoints.size(); i++)
    {
        std::cout << i << ":" << subCurves[1].controlPoints[i].X << " , " << subCurves[1].controlPoints[i].Y << " , " << subCurves[1].controlPoints[i].Z << std::endl;
    }*/
}



