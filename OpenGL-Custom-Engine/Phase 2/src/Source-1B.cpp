//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1B της OpenGL
//
//ΑΜ:2905                     Όνομα:Παναγιώτης Παρασκευόπουλος

//*********************************

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;

/// <summary>
/// //////////////////////////////////////////////
/// </summary>
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

////////////////////////////////////////////// <summary>
/// Add camera function here
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 20.0f); // Camera in World Space
glm::vec3 cameraCenterPoint = glm::vec3(0.0f, 0.0f, 0.25f); // and looks at the origin
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f); // Head is up 
glm::mat4 newView; // Initialize a view model matrix

void camera_function()
{
	glm::vec3 worldCenterPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	float zoomingStep = 0.01f;

	float rotationAngle = 0.01f;

	// Rotate on x axis means that the coords y,z of the yz plane change while x coord stays the same
	glm::mat3 rotationMatrixX = glm::mat3(
		1, 0, 0,
		0, cos(rotationAngle), -sin(rotationAngle),
		0, sin(rotationAngle), cos(rotationAngle)
	);
	
	// Rotate on y axis means that the coords x,z of the xz plane change while y coord stays the same
	glm::mat3 rotationMatrixY = glm::mat3(
		cos(rotationAngle), 0, sin(rotationAngle),
		0, 1, 0,
		-sin(rotationAngle), 0, cos(rotationAngle)
	);

	glm::vec3 normalizedVector = glm::normalize(cameraPosition - worldCenterPoint);

	// Zoom in
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		cameraPosition -= zoomingStep * normalizedVector;
	}
	// Zoom out
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		cameraPosition += zoomingStep * normalizedVector;
	}
	
	// Περιστροφή γύρω από τον άξονα X (counterclockwise)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition = rotationMatrixX * cameraPosition;
		upVector = rotationMatrixX * upVector; // Ανανέωση του upVector
	}

	// Περιστροφή γύρω από τον άξονα X (clockwise)
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		cameraPosition = cameraPosition * rotationMatrixX;
		upVector = upVector * rotationMatrixX; // Ανανέωση του upVector
		}

	// Περιστροφή γύρω από τον άξονα Y (counterclockwise)
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraPosition = rotationMatrixY * cameraPosition;
		upVector = rotationMatrixY * upVector; // Ανανέωση του upVector
	}

	// Περιστροφή γύρω από τον άξονα Y (clockwise)
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		cameraPosition = cameraPosition * rotationMatrixY;
		upVector = upVector * rotationMatrixY; // Ανανέωση του upVector
	}

	// Calculate new camera matrix (view matrix)
	glm::mat4 view = glm::lookAt(
		cameraPosition, // Νέα θέση κάμερας
		cameraCenterPoint, // Το σημείο που κοιτάζει η κάμερα παραμένει ίδιο
		upVector // Ανανέωση του upVector για σωστό προσανατολισμό
	);

	

	newView = view;
}


/////////////////////////////////////////////////

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
///////////////////////////////////////////////////



int main(void)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(950, 950, u8"Άσκηση 1Β - 2024", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// background color black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders

	GLuint programID = LoadShaders("P1BVertexShader.vertexshader", "P1BFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		cameraPosition,		// Camera is initially at (0.0, 0.0, 20.0) in world space
		cameraCenterPoint,	// Camera looks at (0.0, 0.0, 0.25)
		upVector			// Camera is using an up vector of (0.0, 1.0, 0.0) - head looks at z axis
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
	
	const int maze[10][10] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{0, 0, 1, 1, 1, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
		{1, 0, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
		{1, 0, 1, 0, 1, 1, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	};

	std::vector<GLfloat> maze_vertices;

	float size = 1.0f;

	// Διατρέχουμε τον λαβύρινθο και δημιουργούμε τους κύβους
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (maze[i][j] == 1) {
				// Κεντράρουμε τον λαβύρινθο μετατοπίζοντάς τον ώστε να είναι κεντραρισμένος στο (0,0)
				float x = (j - 5) * size; // Μετατόπιση κατά τον άξονα x (για 10x10, χρησιμοποιούμε -5)
				float y = ((9 - i) - 5) * size; // Αντιστροφή του άξονα y για να αντιστρέψουμε κάθετα τον λαβύρινθο
				float z = 0.0f; // Εδώ μπορούμε να αφήσουμε το z να είναι μηδέν για το κάτω μέρος του κύβου

				// Δημιουργούμε τα 12 τρίγωνα που σχηματίζουν τον κύβο
				// Ο κύβος έχει 6 πλευρές, κάθε πλευρά σχηματίζεται από 2 τρίγωνα

				// Πρόσωπο μπροστά
				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y);        maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);

				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);
				maze_vertices.push_back(x);        maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);

				// Πρόσωπο πίσω
				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y);        maze_vertices.push_back(z);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z);

				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z);
				maze_vertices.push_back(x);        maze_vertices.push_back(y + size); maze_vertices.push_back(z);

				// Πρόσωπο αριστερά
				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z);
				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z + size);
				maze_vertices.push_back(x);        maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);

				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z);
				maze_vertices.push_back(x);        maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);
				maze_vertices.push_back(x);        maze_vertices.push_back(y + size); maze_vertices.push_back(z);

				// Πρόσωπο δεξιά
				maze_vertices.push_back(x + size); maze_vertices.push_back(y);        maze_vertices.push_back(z);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y);        maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);

				maze_vertices.push_back(x + size); maze_vertices.push_back(y);        maze_vertices.push_back(z);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z);

				// Πρόσωπο πάνω
				maze_vertices.push_back(x);        maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z);

				maze_vertices.push_back(x);        maze_vertices.push_back(y + size); maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y + size); maze_vertices.push_back(z);
				maze_vertices.push_back(x);        maze_vertices.push_back(y + size); maze_vertices.push_back(z);

				// Πρόσωπο κάτω
				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y);        maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y);        maze_vertices.push_back(z);

				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z + size);
				maze_vertices.push_back(x + size); maze_vertices.push_back(y);        maze_vertices.push_back(z);
				maze_vertices.push_back(x);        maze_vertices.push_back(y);        maze_vertices.push_back(z);
			}
		}
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, maze_vertices.size() * sizeof(GLfloat), maze_vertices.data(), GL_STATIC_DRAW);



	// Δεδομένα χρώματος για τα τρίγωνα (μπλε)
	std::vector<GLfloat> maze_colors;

	// Χρώμα μπλε (RGB = 0, 0, 1, Διαφάνεια = 0.0)
	GLfloat blueColor[] = { 0.0f, 0.0f, 1.0f, 0.0};

	// Δημιουργία χρωμάτων για κάθε τρίγωνο του κύβου
	for (int i = 0; i < maze_vertices.size() / 3; ++i) {
		maze_colors.push_back(blueColor[0]);
		maze_colors.push_back(blueColor[1]);
		maze_colors.push_back(blueColor[2]);
		maze_colors.push_back(blueColor[3]);
	}

	// Δημιουργία buffer για τα χρώματα
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, maze_colors.size() * sizeof(GLfloat), maze_colors.data(), GL_STATIC_DRAW);

	// Αρχικές συντεταγμένες κελιού του κύβου Α (ξεκινάει στο [2, 0])
	int player_x = 0; // στήλη 0
	int player_y = 2; // γραμμή 2

	// Συνάρτηση για να ενημερώνει τις κορυφές του κύβου Α ανάλογα με τη θέση του
	auto update_cube_A_vertices = [&](int x, int y) -> std::vector<GLfloat> {
		float cell_size = 1.0f;
		float center_x = (x - 5) * cell_size + 0.25f;
		float center_y = ((9 - y) - 5) * cell_size + 0.25f;
		float center_z = 0.25f;  

		// Το μέγεθος του κύβου
		float size = 0.5f;  // Μισό του μήκους της ακμής του κύβου (κύβος 0.5x0.5x0.5)

		return {
			// Εμπρόσθια πλευρά (6 κορυφές, 2 τρίγωνα)
			center_x,        center_y,        center_z + size,  // κάτω αριστερά
			center_x + size, center_y,        center_z + size,  // κάτω δεξιά
			center_x + size, center_y + size, center_z + size,  // πάνω δεξιά

			center_x,        center_y,        center_z + size,  // κάτω αριστερά
			center_x + size, center_y + size, center_z + size,  // πάνω δεξιά
			center_x,        center_y + size, center_z + size,  // πάνω αριστερά

			// Πίσω πλευρά (6 κορυφές, 2 τρίγωνα) 
			center_x,        center_y,        center_z,         // κάτω αριστερά
			center_x + size, center_y,        center_z,         // κάτω δεξιά
			center_x + size, center_y + size, center_z,         // πάνω δεξιά

			center_x,        center_y,        center_z,         // κάτω αριστερά
			center_x + size, center_y + size, center_z,         // πάνω δεξιά
			center_x,        center_y + size, center_z,         // πάνω αριστερά

			// Δεξιά πλευρά (6 κορυφές, 2 τρίγωνα)
			center_x + size, center_y,        center_z + size,  // εμπρόσθιο κάτω δεξιά
			center_x + size, center_y + size, center_z + size,  // εμπρόσθιο πάνω δεξιά
			center_x + size, center_y + size, center_z,         // πίσω πάνω δεξιά

			center_x + size, center_y,        center_z + size,  // εμπρόσθιο κάτω δεξιά
			center_x + size, center_y + size, center_z,         // πίσω πάνω δεξιά
			center_x + size, center_y,        center_z,         // πίσω κάτω δεξιά

			// Αριστερή πλευρά (6 κορυφές, 2 τρίγωνα)
			center_x,        center_y,        center_z + size,  // εμπρόσθιο κάτω αριστερά
			center_x,        center_y + size, center_z + size,  // εμπρόσθιο πάνω αριστερά
			center_x,        center_y + size, center_z,         // πίσω πάνω αριστερά

			center_x,        center_y,        center_z + size,  // εμπρόσθιο κάτω αριστερά
			center_x,        center_y + size, center_z,         // πίσω πάνω αριστερά
			center_x,        center_y,        center_z,         // πίσω κάτω αριστερά

			// Άνω πλευρά (6 κορυφές, 2 τρίγωνα)
			center_x,        center_y + size, center_z + size,  // εμπρόσθιο πάνω αριστερά
			center_x + size, center_y + size, center_z + size,  // εμπρόσθιο πάνω δεξιά
			center_x + size, center_y + size, center_z,         // πίσω πάνω δεξιά

			center_x,        center_y + size, center_z + size,  // εμπρόσθιο πάνω αριστερά
			center_x + size, center_y + size, center_z,         // πίσω πάνω δεξιά
			center_x,        center_y + size, center_z,         // πίσω πάνω αριστερά

			// Κάτω πλευρά (6 κορυφές, 2 τρίγωνα)
			center_x,        center_y,        center_z + size,  // εμπρόσθιο κάτω αριστερά
			center_x + size, center_y,        center_z + size,  // εμπρόσθιο κάτω δεξιά
			center_x + size, center_y,        center_z,         // πίσω κάτω δεξιά

			center_x,        center_y,        center_z + size,  // εμπρόσθιο κάτω αριστερά
			center_x + size, center_y,        center_z,         // πίσω κάτω δεξιά
			center_x,        center_y,        center_z          // πίσω κάτω αριστερά
		};

	};

	// Αρχικές κορυφές του κύβου Α
	std::vector<GLfloat> cube_A_vertices = update_cube_A_vertices(player_x, player_y);

	// Χρώμα για κάθε κορυφή (κίτρινο)
	std::vector<GLfloat> cube_A_colors = {
		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f, 
		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f, 
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f, 

		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f,  
		1.0f, 1.0f, 0.0f, 0.0f, 

		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 0.0f
	};

	// Δημιουργία του buffer για τις κορυφές του κύβου
	GLuint cube_vertexbuffer;
	glGenBuffers(1, &cube_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, cube_A_vertices.size() * sizeof(GLfloat), cube_A_vertices.data(), GL_STATIC_DRAW);

	// Δημιουργία του buffer για το χρώμα του κύβου
	GLuint cube_colorbuffer;
	glGenBuffers(1, &cube_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cube_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, cube_A_colors.size() * sizeof(GLfloat), cube_A_colors.data(), GL_STATIC_DRAW);
	

	bool key_l_pressed = false;
	bool key_j_pressed = false;
	bool key_k_pressed = false;
	bool key_i_pressed = false;

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		// Use our shader
		glUseProgram(programID);

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera function
		camera_function();
		
		glm::mat4 Model = glm::mat4(1.0f);
		
		glm::mat4 MVP = Projection * newView * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Ανάγνωση πλήκτρων για την κίνηση 
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !key_l_pressed) {
			if (player_x + 1 < 10 && maze[player_y][player_x + 1] == 0) { // κίνηση δεξιά
				player_x++;
			}
			else if (player_x + 1 >= 10) {
					player_x = 0;
					player_y = 2;
				}
			
			key_l_pressed = true; // Σημείωσε ότι το πλήκτρο είναι τώρα πατημένο
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
			key_l_pressed = false; // Επαναφορά όταν το πλήκτρο απελευθερωθεί
		}

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && !key_j_pressed) {
			if (player_x - 1 >= 0 && maze[player_y][player_x - 1] == 0) { // κίνηση αριστερά
				player_x--;
			}
			else if (player_x - 1 < 0) {
					player_x = 9;
					player_y = 7;
				}
			
			key_j_pressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) {
			key_j_pressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !key_k_pressed) {
			if (player_y + 1 < 10 && maze[player_y + 1][player_x] == 0) { // κίνηση κάτω
				player_y++;
			}
			key_k_pressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE) {
			key_k_pressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS && !key_i_pressed) {
			if (player_y - 1 >= 0 && maze[player_y - 1][player_x] == 0) { // κίνηση πάνω
				player_y--;
			}
			key_i_pressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) {
			key_i_pressed = false;
		}
		


		// Ενημέρωση κορυφών του κυβου Α
		cube_A_vertices = update_cube_A_vertices(player_x, player_y);

		// Ενημέρωση του buffer με τις νέες κορυφές
		glBindBuffer(GL_ARRAY_BUFFER, cube_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, cube_A_vertices.size() * sizeof(GLfloat), cube_A_vertices.data(), GL_STATIC_DRAW);

		// Attribute buffers for maze
		// 1rst attribute buffer : vertices for maze
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  
			3,                 
			GL_FLOAT,           
			GL_FALSE,           
			0,                  
			(void*)0            
		);

		// 2nd attribute buffer : colors for maze
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                
			4,                                // size
			GL_FLOAT,                        
			GL_FALSE,                         
			0,                                
			(void*)0                          
		);

		// Draw triangles 
		glDrawArrays(GL_TRIANGLES, 0, maze_vertices.size() / 3);
		glDisableVertexAttribArray(0);

		// Attribute buffers for cube A
		// 1rst attribute buffer : vertices for cube A
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, cube_vertexbuffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// 2nd attribute buffer : colors for cube A
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, cube_colorbuffer);
		glVertexAttribPointer(
			1,
			4,                                // size
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// Draw triangles 
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the SPACE key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

