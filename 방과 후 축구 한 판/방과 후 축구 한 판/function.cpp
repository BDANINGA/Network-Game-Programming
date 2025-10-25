#define _CRT_SECURE_NO_WARNINGS
#include "function.h"
#include "Camera.h"
#include "Light.h"

extern Camera camera;
extern Light light;

#define M_PI 3.14159265358979323846

// �Լ���
void windowToOpenGL(int window_x, int window_y, int window_width, int window_height, float& gl_x, float& gl_y) {
    // OpenGL ��ǥ���� x�� ��ȯ: ������ ��ǥ�� -1.0 ~ 1.0 ���̷� ����ȭ
    gl_x = (2.0f * window_x) / window_width - 1.0f;

    // OpenGL ��ǥ���� y�� ��ȯ: ������ ��ǥ�� ���Ʒ��� �ݴ��̹Ƿ� y�� ��ȯ
    gl_y = 1.0f - (2.0f * window_y) / window_height;
}
char* filetobuf(const char* file)
{
    FILE* fptr;
    long length;
    char* buf;
    fptr = fopen(file, "rb"); // Open file for reading
    if (!fptr) // Return NULL on failure
        return NULL;
    fseek(fptr, 0, SEEK_END); // Seek to the end of the file
    length = ftell(fptr); // Find out how many bytes into the file we are
    buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
    fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
    fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
    fclose(fptr); // Close the file
    buf[length] = 0; // Null terminator
    return buf; // Return the buffer 
}
void deleteTexture(GLuint textureID) {
    glDeleteTextures(1, &textureID);
}
bool checkSegmentCollision(glm::vec3 start, glm::vec3 end, glm::vec3 goalPos, glm::vec3 goalScale) {
    glm::vec3 boxMin = goalPos - goalScale;
    glm::vec3 boxMax = goalPos + goalScale;

    for (int i = 0; i < 3; ++i) {
        float boxMinCoord = boxMin[i];
        float boxMaxCoord = boxMax[i];

        if (start[i] < boxMinCoord && end[i] < boxMinCoord || start[i] > boxMaxCoord && end[i] > boxMaxCoord)
            return false;

        if (start[i] < boxMinCoord || end[i] > boxMaxCoord) {
            float t = (boxMinCoord - start[i]) / (end[i] - start[i]);
            if (t > 0.0f && t < 1.0f) {
                return true;
            }
        }
    }

    return true;
}

// tiemer
void TimerFunction(int value)
{
    glutTimerFunc(25, TimerFunction, 1);
    glutPostRedisplay();
}

// shader
void make_vertexShaders()
{
    GLchar* vertexSource;
    //--- ���ؽ� ���̴� �о� �����ϰ� ������ �ϱ�
    //--- filetobuf: ��������� �Լ��� �ؽ�Ʈ�� �о ���ڿ��� �����ϴ� �Լ�
    vertexSource = filetobuf("vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
        return;
    }
}
void make_fragmentShaders()
{
    GLchar* fragmentSource;
    //--- �����׸�Ʈ ���̴� �о� �����ϰ� �������ϱ�
    fragmentSource = filetobuf("fragment.glsl"); // �����׼��̴� �о����
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    GLint result;
    GLchar errorLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << "ERROR: frag_shader ������ ����\n" << errorLog << std::endl;
        return;
    }
}
GLuint make_shaderProgram()
{
    GLuint shaderID;
    shaderID = glCreateProgram(); //--- ���̴� ���α׷� �����
    glAttachShader(shaderID, vertexShader); //--- ���̴� ���α׷��� ���ؽ� ���̴� ���̱�
    glAttachShader(shaderID, fragmentShader); //--- ���̴� ���α׷��� �����׸�Ʈ ���̴� ���̱�
    glLinkProgram(shaderID); //--- ���̴� ���α׷� ��ũ�ϱ�
    glDeleteShader(vertexShader); //--- ���̴� ��ü�� ���̴� ���α׷��� ��ũ��������, ���̴� ��ü ��ü�� ���� ����
    glDeleteShader(fragmentShader);
    GLint result;
    GLchar errorLog[512];
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // ---���̴��� �� ����Ǿ����� üũ�ϱ�
    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
        return false;
    }
    glUseProgram(shaderID); //--- ������� ���̴� ���α׷� ����ϱ�
    //--- ���� ���� ���̴����α׷� ���� �� �ְ�, �� �� �Ѱ��� ���α׷��� ����Ϸ���
    //--- glUseProgram �Լ��� ȣ���Ͽ� ��� �� Ư�� ���α׷��� �����Ѵ�.
    //--- ����ϱ� ������ ȣ���� �� �ִ�.
    return shaderID;
}

// obj ������ �Ľ�
ObjData parseObj(const std::string& filePath) {
    ObjData data;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") { // Vertex
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            data.vertices.push_back(vertex);
        }
        else if (prefix == "vt") { // Texture Coordinate
            TextureCoord texCoord;
            iss >> texCoord.u >> texCoord.v;
            data.texCoords.push_back(texCoord);
        }
        else if (prefix == "vn") { // Normal
            Normal normal;
            iss >> normal.nx >> normal.ny >> normal.nz;
            data.normals.push_back(normal);
        }
        else if (prefix == "f") { // Face
            Face face;
            std::string vertexData;
            while (iss >> vertexData) {
                int vIdx = 0, tIdx = 0, nIdx = 0;
                std::replace(vertexData.begin(), vertexData.end(), '/', ' ');
                std::istringstream vertexStream(vertexData);
                vertexStream >> vIdx;
                if (vertexStream.peek() == ' ') vertexStream >> tIdx;
                if (vertexStream.peek() == ' ') vertexStream >> nIdx;
                face.vertices.emplace_back(vIdx - 1, tIdx - 1, nIdx - 1); // Convert to 0-based index
            }
            data.faces.push_back(face);
        }
    }

    file.close();
    return data;
}
void convertToGLArrays(const ObjData& objData, std::vector<GLfloat>& vertexArray, std::vector<GLfloat>& colorArray, std::vector<GLfloat>& normalArray, std::vector<GLfloat>& texCoordArray) {
    for (const auto& face : objData.faces) {
        // �� Face�� ���� ������ �ﰢ�� ������ �и�
        for (size_t i = 1; i + 1 < face.vertices.size(); ++i) {
            // ù ��° ����
            int vIdx1 = std::get<0>(face.vertices[0]);
            int nIdx1 = std::get<2>(face.vertices[0]);
            int tIdx1 = std::get<1>(face.vertices[0]); // �ؽ��� ��ǥ �ε���

            // �� ��° ����
            int vIdx2 = std::get<0>(face.vertices[i]);
            int nIdx2 = std::get<2>(face.vertices[i]);
            int tIdx2 = std::get<1>(face.vertices[i]); // �ؽ��� ��ǥ �ε���

            // �� ��° ����
            int vIdx3 = std::get<0>(face.vertices[i + 1]);
            int nIdx3 = std::get<2>(face.vertices[i + 1]);
            int tIdx3 = std::get<1>(face.vertices[i + 1]); // �ؽ��� ��ǥ �ε���

            // ���� ��ǥ �߰�
            vertexArray.push_back(objData.vertices[vIdx1].x);
            vertexArray.push_back(objData.vertices[vIdx1].y);
            vertexArray.push_back(objData.vertices[vIdx1].z);

            vertexArray.push_back(objData.vertices[vIdx2].x);
            vertexArray.push_back(objData.vertices[vIdx2].y);
            vertexArray.push_back(objData.vertices[vIdx2].z);

            vertexArray.push_back(objData.vertices[vIdx3].x);
            vertexArray.push_back(objData.vertices[vIdx3].y);
            vertexArray.push_back(objData.vertices[vIdx3].z);

            // ���� ���� �߰�
            colorArray.insert(colorArray.end(), {
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f
            });

            // ���� ���� �߰�
            if (nIdx1 >= 0) {
                normalArray.push_back(objData.normals[nIdx1].nx);
                normalArray.push_back(objData.normals[nIdx1].ny);
                normalArray.push_back(objData.normals[nIdx1].nz);
            }
            else {
                normalArray.insert(normalArray.end(), { 0.0f, 0.0f, 0.0f });
            }

            if (nIdx2 >= 0) {
                normalArray.push_back(objData.normals[nIdx2].nx);
                normalArray.push_back(objData.normals[nIdx2].ny);
                normalArray.push_back(objData.normals[nIdx2].nz);
            }
            else {
                normalArray.insert(normalArray.end(), { 0.0f, 0.0f, 0.0f });
            }

            if (nIdx3 >= 0) {
                normalArray.push_back(objData.normals[nIdx3].nx);
                normalArray.push_back(objData.normals[nIdx3].ny);
                normalArray.push_back(objData.normals[nIdx3].nz);
            }
            else {
                normalArray.insert(normalArray.end(), { 0.0f, 0.0f, 0.0f });
            }

            // �ؽ��� ��ǥ �߰�
            if (tIdx1 >= 0) {
                texCoordArray.push_back(objData.texCoords[tIdx1].u);
                texCoordArray.push_back(objData.texCoords[tIdx1].v);
            }
            else {
                texCoordArray.push_back(0.0f);
                texCoordArray.push_back(0.0f); // �⺻��
            }

            if (tIdx2 >= 0) {
                texCoordArray.push_back(objData.texCoords[tIdx2].u);
                texCoordArray.push_back(objData.texCoords[tIdx2].v);
            }
            else {
                texCoordArray.push_back(0.0f);
                texCoordArray.push_back(0.0f); // �⺻��
            }

            if (tIdx3 >= 0) {
                texCoordArray.push_back(objData.texCoords[tIdx3].u);
                texCoordArray.push_back(objData.texCoords[tIdx3].v);
            }
            else {
                texCoordArray.push_back(0.0f);
                texCoordArray.push_back(0.0f); // �⺻��
            }
        }
    }
}
void loadObj() {
    ObjData data;
    std::cout << "----- obj ������ �Ľ� �� -----" << std::endl;

    data = parseObj("player.obj");
    convertToGLArrays(data, vertexArray[0], colorArray[0], normalArray[0], textureArray[0]);
    PlayerVertexCount = vertexArray[0].size() / 3;

    std::cout << "player �Ϸ�" << std::endl;
    std::cout << "vertexcount - " << PlayerVertexCount << std::endl;

    data = parseObj("ball.obj");
    convertToGLArrays(data, vertexArray[1], colorArray[1], normalArray[1], textureArray[1]);
    BallVertexCount = vertexArray[1].size() / 3;

    std::cout << "ball �Ϸ�" << std::endl;
    std::cout << "vertexcount - " << BallVertexCount << std::endl;

    data = parseObj("cube.obj");
    convertToGLArrays(data, vertexArray[2], colorArray[2], normalArray[2], textureArray[2]);
    GoalPostVertexCount = vertexArray[2].size() / 3;

    std::cout << "��� �Ϸ�" << std::endl;

    std::cout << "----- obj ������ �Ľ� �Ϸ� -----" << std::endl;
}

// bind array
void InitBuffer()
{
    // ------------------------------------------------------------------------------------
    // vao_player
    glGenVertexArrays(1, &vao_player); //--- VAO �� �����ϰ� �Ҵ��ϱ�
    glBindVertexArray(vao_player);

    glGenBuffers(4, vbo_player); //--- 4���� VBO�� �����ϰ� �Ҵ��ϱ�
    glBindBuffer(GL_ARRAY_BUFFER, vbo_player[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexArray[0].size() * sizeof(GLfloat), vertexArray[0].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_player[1]);
    glBufferData(GL_ARRAY_BUFFER, colorArray[0].size() * sizeof(GLfloat), colorArray[0].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_player[2]);
    glBufferData(GL_ARRAY_BUFFER, normalArray[0].size() * sizeof(GLfloat), normalArray[0].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_player[3]);
    glBufferData(GL_ARRAY_BUFFER, textureArray[0].size() * sizeof(GLfloat), textureArray[0].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    // -----------------------------------------------------------------------------------------------------------
    // vao_ball
    glGenVertexArrays(1, &vao_ball); //--- VAO �� �����ϰ� �Ҵ��ϱ�
    glBindVertexArray(vao_ball);

    glGenBuffers(4, vbo_ball); //--- 4���� VBO�� �����ϰ� �Ҵ��ϱ�
    glBindBuffer(GL_ARRAY_BUFFER, vbo_ball[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexArray[1].size() * sizeof(GLfloat), vertexArray[1].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ball[1]);
    glBufferData(GL_ARRAY_BUFFER, colorArray[1].size() * sizeof(GLfloat), colorArray[1].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ball[2]);
    glBufferData(GL_ARRAY_BUFFER, normalArray[1].size() * sizeof(GLfloat), normalArray[1].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ball[3]);
    glBufferData(GL_ARRAY_BUFFER, textureArray[1].size() * sizeof(GLfloat), textureArray[1].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    // -----------------------------------------------------------------------------------------------------------
    // vao_goalpost
    glGenVertexArrays(1, &vao_goalpost); //--- VAO �� �����ϰ� �Ҵ��ϱ�
    glBindVertexArray(vao_goalpost);

    glGenBuffers(4, vbo_goalpost); //--- 4���� VBO�� �����ϰ� �Ҵ��ϱ�
    glBindBuffer(GL_ARRAY_BUFFER, vbo_goalpost[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexArray[2].size() * sizeof(GLfloat), vertexArray[2].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_goalpost[1]);
    glBufferData(GL_ARRAY_BUFFER, colorArray[2].size() * sizeof(GLfloat), colorArray[2].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_goalpost[2]);
    glBufferData(GL_ARRAY_BUFFER, normalArray[2].size() * sizeof(GLfloat), normalArray[2].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_goalpost[3]);
    glBufferData(GL_ARRAY_BUFFER, textureArray[2].size() * sizeof(GLfloat), textureArray[2].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(3);

    // ���� ����
    result = FMOD::System_Create(&ssystem);
    if (result != FMOD_OK)
        exit(0);
    ssystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
    ssystem->createSound("football.mp3", FMOD_LOOP_NORMAL, 0, &s_bgm);
    ssystem->createSound("goal.mp3", FMOD_LOOP_OFF, 0, &s_goal);
    ssystem->createSound("touch.mp3", FMOD_LOOP_OFF, 0, &s_touch);
    ssystem->createSound("shoot.mp3", FMOD_LOOP_OFF, 0, &s_shoot);

    // ��� ���� ���� �ݺ�
    ssystem->playSound(s_bgm, 0, false, &c_bgm);
    glutTimerFunc(25, TimerFunction, 1);

}

// BMP ���� �ε�
GLuint loadBMP(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        std::cout << "Failed to open BMP file: " << filepath << std::endl;
        return 0;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file); // BMP ��� �б�

    // BMP ���� ����
    if (header[0] != 'B' || header[1] != 'M') {
        std::cout << "Not a valid BMP file!" << std::endl;
        fclose(file);
        return 0;
    }

    // �̹��� ũ�� ���� ����
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int imageSize = *(int*)&header[34];

    if (imageSize == 0) imageSize = width * height * 3; // 24��Ʈ BMP�� ���
    unsigned char* data = new unsigned char[imageSize];

    // �̹��� ������ �б�
    fread(data, sizeof(unsigned char), imageSize, file);
    fclose(file);

    // �ؽ�ó ����
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // �ؽ�ó ������ ���ε�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // �ؽ�ó ���͸� ����
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    delete[] data;
    return textureID;
}

// ��ȯ
void viewTransform() {
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camera.getPosition(), camera.getDirection(), camera.getUp());
    unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "viewTransform");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}
void projectionTransform() {
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
    projection = glm::translate(projection, glm::vec3(0.0, 0.0, -4.0));
    unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projectionTransform");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}
void make_Light() {
    unsigned int ambientStrength = glGetUniformLocation(shaderProgramID, "ambientStrength");
    glUniform1f(ambientStrength, light.getAmbientlight());
    unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos");
    glUniform3f(lightPosLocation, light.getPosition().x, light.getPosition().y, light.getPosition().z);
    unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor");
    glUniform3f(lightColorLocation, light.getColor().x, light.getColor().y, light.getColor().z);
    unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos");
    glUniform3f(viewPosLocation, light.getViewPos().x, light.getViewPos().y, light.getViewPos().z);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���� �����(������)
void MakeShape(GLfloat arr[][3], GLfloat normal[][3], GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, int first_index, std::string shape) {
    if (shape == "cube") {
        // 8���� ������ ����
        GLfloat vertices[8][3] = {
            {x1, y1, z1},  // 0�� ������ (�ּ� ��ǥ)
            {x2, y1, z1},  // 1�� ������
            {x2, y2, z1},  // 2�� ������
            {x1, y2, z1},  // 3�� ������
            {x1, y1, z2},  // 4�� ������
            {x2, y1, z2},  // 5�� ������
            {x2, y2, z2},  // 6�� ������
            {x1, y2, z2}   // 7�� ������ (�ִ� ��ǥ)
        };

        int faces[6][6] = {
     {0, 3, 1, 1, 2, 3},  // �ո�
     {4, 7, 5, 5, 6, 7},  // �޸�
     {0, 4, 3, 3, 7, 4},  // ���ʸ�
     {1, 2, 5, 5, 6, 2},  // �����ʸ�
     {3, 7, 2, 2, 6, 7},  // ���ʸ�
     {0, 1, 4, 4, 5, 1}   // �Ʒ��ʸ�
        };

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                int index = first_index + i * 6 + j;
                arr[index][0] = vertices[faces[i][j]][0];
                arr[index][1] = vertices[faces[i][j]][1];
                arr[index][2] = vertices[faces[i][j]][2];
                normal[index][0] = vertices[faces[i][j]][0];
                normal[index][1] = vertices[faces[i][j]][1];
                normal[index][2] = vertices[faces[i][j]][2];
            }
        }
    }
    else if (shape == "pyramid") {
        // �Ƕ�̵��� ������ ����
        GLfloat vertices[5][3] = {
            {x1, y1, z1},  // 0�� ������ (�ٴ�)
            {x2, y1, z1},  // 1�� ������
            {x2, y1, z2},  // 2�� ������
            {x1, y1, z2},  // 3�� ������
            {(x1 + x2) / 2, y2, (z1 + z2) / 2}  // 4�� ������ (�Ƕ�̵� ������)
        };

        // �Ƕ�̵��� �� ����
        int faces[4][3] = {
            {0, 1, 4},  // �ﰢ�� 1
            {1, 2, 4},  // �ﰢ�� 2
            {2, 3, 4},  // �ﰢ�� 3
            {3, 0, 4}   // �ﰢ�� 4
        };

        // �ﰢ�� ���� ���� �� ���� ���
        for (int i = 0; i < 4; ++i) {  // �� ���� �鿡 ���� �ݺ�
            // ���� ������ ��������
            GLfloat* p0 = vertices[faces[i][0]];
            GLfloat* p1 = vertices[faces[i][1]];
            GLfloat* p2 = vertices[faces[i][2]];

            // �� ���� ���
            GLfloat v1[3] = { p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2] };
            GLfloat v2[3] = { p2[0] - p0[0], p2[1] - p0[1], p2[2] - p0[2] };

            // ���� ��� (���� ����)
            GLfloat n[3] = {
                v1[1] * v2[2] - v1[2] * v2[1],  // ���� ���� ���� ����
                v1[2] * v2[0] - v1[0] * v2[2],
                v1[0] * v2[1] - v1[1] * v2[0]
            };

            // ���� ���� ���� (����ȭ�� ���̴����� ó����)
            for (int j = 0; j < 3; ++j) {
                normal[first_index / 3 + i * 3 + j][0] = n[0];
                normal[first_index / 3 + i * 3 + j][1] = n[1];
                normal[first_index / 3 + i * 3 + j][2] = n[2];
            }

            // ���� ������ ����
            for (int j = 0; j < 3; ++j) {
                arr[first_index + i * 3 + j][0] = vertices[faces[i][j]][0];
                arr[first_index + i * 3 + j][1] = vertices[faces[i][j]][1];
                arr[first_index + i * 3 + j][2] = vertices[faces[i][j]][2];
            }
        }

        // �ٴڸ� �߰� (������ �Ʒ��� ����)
        GLfloat base_normal[3] = { 0.0f, -1.0f, 0.0f };
        int base_faces[2][3] = {
            {0, 1, 2},  // ù ��° �ﰢ��
            {0, 2, 3}   // �� ��° �ﰢ��
        };

        for (int i = 0; i < 2; ++i) {  // �ٴ��� �� �ﰢ��
            for (int j = 0; j < 3; ++j) {
                int idx = first_index + 12 + i * 3 + j;
                arr[idx][0] = vertices[base_faces[i][j]][0];
                arr[idx][1] = vertices[base_faces[i][j]][1];
                arr[idx][2] = vertices[base_faces[i][j]][2];

                // �ٴڸ��� ���� ����
                normal[first_index / 3 + 4 + i * 3 + j][0] = base_normal[0];
                normal[first_index / 3 + 4 + i * 3 + j][1] = base_normal[1];
                normal[first_index / 3 + 4 + i * 3 + j][2] = base_normal[2];
            }
        }
    }
    else if (shape == "sphere") {
        MakeSphere(arr, normal, (x1 + x2) / 2, y2, (z1 + z2) / 2, (x2 - x1) / 2, first_index); // ���� �߽ɰ� ������ ���
    }
    else if (shape == "cone") {
        // x1, y1, z1 : ���� �ظ��� �߽�
        // x2, y2, z2 : ���� �������� ��ġ
        // radius : ���� �ظ��� ������
        GLfloat radius = (x2 - x1) / 2;
        GLfloat angleStep = 2 * M_PI / 18;  // ���� ������

        // ������ ������ (�������� y������ ���� y2��ŭ �ö� ��)
        GLfloat apex[3] = { (x1 + x2) / 2, y2, (z1 + z2) / 2 };

        // �ظ� �߽���
        GLfloat baseCenter[3] = { (x1 + x2) / 2, y1, (z1 + z2) / 2 };

        // �ظ��� ������ ������ (36���� ��)
        GLfloat vertices[36][3];
        for (int i = 0; i < 36; ++i) {
            GLfloat angle = i * angleStep;
            vertices[i][0] = baseCenter[0] + radius * cos(angle);  // x ��ǥ
            vertices[i][1] = y1;                                   // y ��ǥ (�ظ��� ������ y ��ǥ)
            vertices[i][2] = baseCenter[2] + radius * sin(angle);  // z ��ǥ
        }

        int index = first_index;

        // ������ �����ϴ� �ﰢ���� (�ظ��� �� ���� �������� ����)
        for (int i = 0; i < 36; ++i) {
            int next = (i + 1) % 36;

            // ù ��° �ﰢ��: ����
            arr[index][0] = vertices[i][0];   // �ظ� �� 1
            arr[index][1] = vertices[i][1];
            arr[index][2] = vertices[i][2];

            arr[index + 1][0] = apex[0];      // ������
            arr[index + 1][1] = apex[1];
            arr[index + 1][2] = apex[2];

            arr[index + 2][0] = vertices[next][0]; // �ظ� �� 2
            arr[index + 2][1] = vertices[next][1];
            arr[index + 2][2] = vertices[next][2];

            index += 3;
        }

        // �ظ��� �����ϴ� �ﰢ���� (�߽����� �� �ظ� ���� �ﰢ������ ����)
        for (int i = 0; i < 36; ++i) {
            int next = (i + 1) % 36;

            // �� ��° �ﰢ��: �ظ�
            arr[index][0] = baseCenter[0];    // �ظ� �߽���
            arr[index][1] = baseCenter[1];
            arr[index][2] = baseCenter[2];

            arr[index + 1][0] = vertices[next][0]; // �ظ� �� 2
            arr[index + 1][1] = vertices[next][1];
            arr[index + 1][2] = vertices[next][2];

            arr[index + 2][0] = vertices[i][0];   // �ظ� �� 1
            arr[index + 2][1] = vertices[i][1];
            arr[index + 2][2] = vertices[i][2];

            index += 3;

        }
    }
    else if (shape == "rectangle") {
        // ���簢���� 4���� ������ ����
        GLfloat vertices[4][3] = {
            {x1, y1, z1},  // 0�� ������
            {x2, y1, z1},  // 1�� ������
            {x2, y2, z2},  // 2�� ������
            {x1, y2, z2}   // 3�� ������
        };

        // ���簢���� �� ���� �ﰢ������ ����
        int faces[2][3] = {
            {0, 1, 2},  // ù ��° �ﰢ�� (0, 1, 2)
            {0, 2, 3}   // �� ��° �ﰢ�� (0, 2, 3)
        };

        // �ﰢ���� �������� arr �迭�� ����
        for (int i = 0; i < 2; ++i) {  // 2���� �ﰢ�� ��
            for (int j = 0; j < 3; ++j) {
                int index = first_index + i * 3 + j;
                arr[index][0] = vertices[faces[i][j]][0];  // x��ǥ
                arr[index][1] = vertices[faces[i][j]][1];  // y��ǥ
                arr[index][2] = vertices[faces[i][j]][2];  // z��ǥ
            }
        }
    }
    else if (shape == "triangle") {
        // x1, y1, z1 : �ﰢ���� ù ��° ������
        // x2, y2, z2 : �ﰢ���� �� ��° ������
        // x3, y3, z3 : �ﰢ���� �� ��° ������

        // �ﰢ���� ������ ����
        GLfloat vertices[3][3] = {
            {x1, y2, z1},  // 0�� ������
            {(x1 + x2) / 2, y1, z1},  // 1�� ������
            {x2, y2, z1}   // 2�� ������
        };

        // �ﰢ���� �������� arr �迭�� ����
        for (int i = 0; i < 3; ++i) {
            arr[first_index + i][0] = vertices[i][0];  // x��ǥ
            arr[first_index + i][1] = vertices[i][1];  // y��ǥ
            arr[first_index + i][2] = vertices[i][2];  // z��ǥ
        }
    }
}
void MakeColor(GLfloat arr[][3], int first_index, int index_count, GLfloat color[3]) {
    for (int j = first_index; j < first_index + index_count; j++) {
        arr[j][0] = color[0];
        arr[j][1] = color[1];
        arr[j][2] = color[2];
    }
}
void MakeSphere(GLfloat arr[][3], GLfloat normal[][3], GLfloat cx, GLfloat cy, GLfloat cz, GLfloat radius, int first_index)
{
    int longitudeSegments = 18;  // �浵 ���� ��
    int latitudeSegments = 9;    // ���� ���� ��
    int vertexCount = (latitudeSegments + 1) * (longitudeSegments + 1);

    // ���� �޸� �Ҵ�
    GLfloat** vertices = new GLfloat * [vertexCount];
    for (int i = 0; i < vertexCount; ++i) {
        vertices[i] = new GLfloat[3];
    }

    // ���� ���� ����
    for (int lat = 0; lat <= latitudeSegments; ++lat) {
        float theta = lat * M_PI / latitudeSegments; // ���� ����
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longitudeSegments; ++lon) {
            float phi = lon * 2 * M_PI / longitudeSegments; // �浵 ����
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            // ���� ��ǥ ���
            vertices[lat * (longitudeSegments + 1) + lon][0] = cx + radius * cosPhi * sinTheta; // x
            vertices[lat * (longitudeSegments + 1) + lon][1] = cy + radius * cosTheta;         // y
            vertices[lat * (longitudeSegments + 1) + lon][2] = cz + radius * sinPhi * sinTheta; // z
        }
    }

    // ���� �� ����
    int index = first_index; // �� �ε��� ���� ��ġ
    for (int lat = 0; lat < latitudeSegments; ++lat) {
        for (int lon = 0; lon < longitudeSegments; ++lon) {
            int first = lat * (longitudeSegments + 1) + lon;
            int second = first + longitudeSegments + 1;

            // �� ���� �ﰢ������ �� ����
            // ù ��° �ﰢ��
            arr[index][0] = vertices[first][0];
            arr[index][1] = vertices[first][1];
            arr[index][2] = vertices[first][2];
            // ���� ���� ���
            normal[index][0] = (vertices[first][0] - cx) / radius;
            normal[index][1] = (vertices[first][1] - cy) / radius;
            normal[index][2] = (vertices[first][2] - cz) / radius;

            arr[index + 1][0] = vertices[second][0];
            arr[index + 1][1] = vertices[second][1];
            arr[index + 1][2] = vertices[second][2];
            // ���� ���� ���
            normal[index + 1][0] = (vertices[second][0] - cx) / radius;
            normal[index + 1][1] = (vertices[second][1] - cy) / radius;
            normal[index + 1][2] = (vertices[second][2] - cz) / radius;

            arr[index + 2][0] = vertices[first + 1][0];
            arr[index + 2][1] = vertices[first + 1][1];
            arr[index + 2][2] = vertices[first + 1][2];
            // ���� ���� ���
            normal[index + 2][0] = (vertices[first + 1][0] - cx) / radius;
            normal[index + 2][1] = (vertices[first + 1][1] - cy) / radius;
            normal[index + 2][2] = (vertices[first + 1][2] - cz) / radius;

            // �� ��° �ﰢ��
            arr[index + 3][0] = vertices[second][0];
            arr[index + 3][1] = vertices[second][1];
            arr[index + 3][2] = vertices[second][2];
            // ���� ���� ���
            normal[index + 3][0] = (vertices[second][0] - cx) / radius;
            normal[index + 3][1] = (vertices[second][1] - cy) / radius;
            normal[index + 3][2] = (vertices[second][2] - cz) / radius;

            arr[index + 4][0] = vertices[second + 1][0];
            arr[index + 4][1] = vertices[second + 1][1];
            arr[index + 4][2] = vertices[second + 1][2];
            // ���� ���� ���
            normal[index + 4][0] = (vertices[second + 1][0] - cx) / radius;
            normal[index + 4][1] = (vertices[second + 1][1] - cy) / radius;
            normal[index + 4][2] = (vertices[second + 1][2] - cz) / radius;

            arr[index + 5][0] = vertices[first + 1][0];
            arr[index + 5][1] = vertices[first + 1][1];
            arr[index + 5][2] = vertices[first + 1][2];
            // ���� ���� ���
            normal[index + 5][0] = (vertices[first + 1][0] - cx) / radius;
            normal[index + 5][1] = (vertices[first + 1][1] - cy) / radius;
            normal[index + 5][2] = (vertices[first + 1][2] - cz) / radius;

            index += 6; // ���� �ﰢ������ �̵�
        }
    }

    // ���� �޸� ����
    for (int i = 0; i < vertexCount; ++i) {
        delete[] vertices[i];
    }
    delete[] vertices;
}
