//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1Α της OpenGL
//
//ΑΜ:2905                         Όνομα:Παναγιώτης Παρασκευόπουλος

//*********************************

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <chrono>


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

//******************
// Η LoadShaders είναι black box για σας

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


int main(void)
{
	// Initialise GLFW
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

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(750, 750, u8"Άσκηση 1Α - 2024", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the Q key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//***********************************************
	// Οι shaders σας είναι οι 
    // ProjectVertexShader.vertexshader
    // ProjectFragmentShader.fragmentshader

	GLuint programID = LoadShaders("ProjectVertexShader.vertexshader", "ProjectFragmentShader.fragmentshader");
	
    ///////////////////////////////////////////////////////////////////////////////////////	
	/**Το παρακάτω το αγνοείτε - είναι τοποθέτηση κάμερας ***/
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	glm::mat4 Projection = glm::perspective(glm::radians(30.0f), 4.0f / 4.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 30), // Camera  in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // 
	);

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model; 
    ///////////////////////////////////////////////////////////////////////////////////////
	//**************************************************
	/// Για βοήθεια το πρόγραμμα αναπαριστά ενα τυχαίο τρίγωνο - εσείς θα πρέπει να βάλετε κορυφές κατάλληλες 
	//  για το δικό σας τρίγωνο.
	//Στην άσκηση αυτή δουλεύετε στις 2 διαστάσεις x,y οπότε η z συνιστώσα θα ειναι πάντα 0.0f
	
	// Ορισμός του λαβυρίνθου (10x10 grid)
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

		// Μέγεθος κάθε τετραγώνου
		float size = 1.0;

		// Διατρέχουμε τον λαβύρινθο και δημιουργούμε τα τετράγωνα
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (maze[i][j] == 1) {
					// Κεντράρουμε τον λαβύρινθο μετατοπίζοντάς τον ώστε να είναι κεντραρισμένος στο (0,0)
					float x = (j - 5) * size; // Μετατόπιση κατά τον άξονα x (για 10x10, χρησιμοποιούμε -5)
					float y = ((9 - i) - 5) * size; // Αντιστροφή του άξονα y για να αντιστρέψουμε κάθετα τον λαβύρινθο

					// Δύο τρίγωνα σχηματίζουν τετράγωνο
					maze_vertices.push_back(x);          maze_vertices.push_back(y);          maze_vertices.push_back(0.0f);
					maze_vertices.push_back(x + size);   maze_vertices.push_back(y);          maze_vertices.push_back(0.0f);
					maze_vertices.push_back(x + size);   maze_vertices.push_back(y + size);   maze_vertices.push_back(0.0f);

					maze_vertices.push_back(x);          maze_vertices.push_back(y + size);   maze_vertices.push_back(0.0f);
					maze_vertices.push_back(x);          maze_vertices.push_back(y);          maze_vertices.push_back(0.0f);
					maze_vertices.push_back(x + size);   maze_vertices.push_back(y + size);   maze_vertices.push_back(0.0f);
				}
			}
		}


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, maze_vertices.size() * sizeof(GLfloat), maze_vertices.data(), GL_STATIC_DRAW);

	
	// Αρχικές συντεταγμένες κελιού του τετραγώνου Α (ξεκινάει στο [2, 0])
	int player_x = 0; // στήλη 0
	int player_y = 2; // γραμμή 2

	// Συνάρτηση για να ενημερώνει τις κορυφές του Α ανάλογα με τη θέση του
	auto update_square_A_vertices = [&](int x, int y) -> std::vector<GLfloat> {
		float cell_size = 1.0f;
		float center_x = (x - 5) * cell_size;
		float center_y = ((9 - y) - 5) * cell_size;

		return {
			center_x + 0.25f, center_y + 0.25f, 0.0f,  // κάτω αριστερή κορυφή
			center_x + 0.75f, center_y + 0.25f, 0.0f,  // κάτω δεξιά κορυφή
			center_x + 0.75f, center_y + 0.75f, 0.0f,  // πάνω δεξιά κορυφή

			center_x + 0.25f, center_y + 0.75f, 0.0f,  // πάνω αριστερή κορυφή
			center_x + 0.75f, center_y + 0.75f, 0.0f,  // πάνω δεξιά κορυφή
			center_x + 0.25f, center_y + 0.25f, 0.0f   // κάτω αριστερή κορυφή
		};
	};

	// Αρχικές κορυφές του τετραγώνου Α
	std::vector<GLfloat> square_A_vertices = update_square_A_vertices(player_x, player_y);

	GLuint square_vertexbuffer;
	glGenBuffers(1, &square_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, square_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, square_A_vertices.size() * sizeof(GLfloat), square_A_vertices.data(), GL_STATIC_DRAW);
	
	bool key_l_pressed = false;
	bool key_j_pressed = false;
	bool key_k_pressed = false;
	bool key_i_pressed = false;

	auto start_time = std::chrono::high_resolution_clock::now();

	
	do {
		
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);  /// Αυτό αφορά την κάμερα  - το αγνοείτε

		// Ανάγνωση πλήκτρων για την κίνηση 
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !key_l_pressed) {
			if (player_x + 1 < 10 && maze[player_y][player_x + 1] == 0) { // κίνηση δεξιά
				player_x++;
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

		// Έλεγχος αν ο παίκτης έφτασε στην έξοδο (κελί [7, 9])
		if (player_x == 9 && player_y == 7) {
			auto end_time = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> elapsed_seconds = end_time - start_time;

			std::cout << "Congratulations, you found the exit!" << std::endl;
			std::cout << "Time taken: " << elapsed_seconds.count() << " Seconds" << std::endl;
			break; // Τερματισμός του loop
		}

		// Ενημέρωση κορυφών του τετραγώνου Α
		square_A_vertices = update_square_A_vertices(player_x, player_y);

		// Ενημέρωση του buffer με τις νέες κορυφές
		glBindBuffer(GL_ARRAY_BUFFER, square_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, square_A_vertices.size() * sizeof(GLfloat), square_A_vertices.data(), GL_STATIC_DRAW);

		// Σχεδίαση του λαβυρινθου
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0, must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, maze_vertices.size() / 3);
		glDisableVertexAttribArray(0);

		// Σχεδίαση του τετραγώνου Α
		glBindBuffer(GL_ARRAY_BUFFER, square_vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0, must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);  // 6 κορυφές για το τετράγωνο (2 τρίγωνα)
		glDisableVertexAttribArray(0);


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} 
	while (glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&  glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &square_vertexbuffer);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

