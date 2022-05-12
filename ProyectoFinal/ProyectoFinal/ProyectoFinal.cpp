#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Otros includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Otros Bibliotecas
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Carga de modelos
#include "SOIL2/SOIL2.h"

// Declaración de funciones
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode); //Función que contiene las teclas que contiene una acción
void MouseCallback(GLFWwindow *window, double xPos, double yPos); //Función que la posición del cursor del ratón para el movimiento de la cámara
void DoMovement(); // Función que permite mueve la posición de la cámara según las teclas que precione el usuario
void RupiaAnim(); // Función que contiene las variables y calculos para realizar la animación de la rupia
void AlasAnim(); // Función que contiene las variables y calvulos para realizar la animación de las alas de Navi

// Dimensión de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Cámera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Atributos de la luz
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

//-------------------------------------------------Declaración de variables para animación--------|
//--------------------Luz
bool LuzAnim = false;		//Indica la activació y desactivación de la animación de la luz
float LuzDirY = 0.0f;		//Sirve para dar la dirección en Y de la luz SpotLight de la casa

//-------------------Silla
bool SillaAnim = false;		//Indica la activació y desactivación de la animación de la silla
float SillaMov = 0.0f;		//Sirve para contar la unidades que se traslada en Z la silla

//-------------------Cajon
bool CajonAnim = false;		//Indica la activació y desactivación de la animación del cajón
float CajonMov = 0.0f;		//Sirve para contar la unidades que se traslada en Z del cajón

//------------------Rupia
float RupiaMovY = 0.0f;		//Cuenta las unidades de traslación en Y de la rupia y sirve en función de RupiaMovX
float RupiaMovX = 0.0f;		//Cuenta las unidades de traslación en X de la rupia para el primer rebote
float RupiaMovX2 = 0.0f;	//Sirve como referencia para la ecuación de paralabola para el segundo rebote
float RupiaMovZ = 0.0f;		//Cuenta las unidades de traslación en Z de la rupia
float rotKit = 0.0f;		//Sirve para que la rupia rote en Y

bool RupiaCir = false;		//Indica la activación de la animación de la rupia (vuelve a salir del jarrón cuando es true)
bool RupiaRec1 = true;		//Acitva el primer recorrido de la animación transladando la rupia en Y postivo hasta cierto punto
bool RupiaRec2 = false;		//Acitva el segundo recorrido de la animación haciendo el efecto de rebote calculado por medio de parabola
bool RupiaRec3 = false;		//Acitva el tercer recorrido de la animación haciendo un segundo rebote por medio de otra parabola
bool RupiaRec4 = false;		//Activa el cuarto recorrido de la animación dejando que la rupia gira una vez que terminó el rebote final
 
//-----------------Alas de Navi
float tiempo;				//Sirve como varible independiente en la función seno del archivo anim.vs
bool AlasRot = true;		//Escencial para determinar el incremento y decremento de las variables AlasRotDer y AlasRotIzq
float AlasRotDer = 0.0f;	//Giran las alas derechas de Navi en dirección de Y
float AlasRotIzq = 0.0f;	////Giran las alas izquierdas de Navi en dirección de Y

// Posicion de la Point Light
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 3.1f, 0.0f),
};


float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// Deltatime
GLfloat deltaTime = 0.0f;	// Tiempo entre el frame actual y el último frame
GLfloat lastFrame = 0.0f;  	// Tiempo del ultimo frame

int main()
{
	// Init GLFW
	glfwInit();

	// Crea un objeto GLFWwindow que se pueda usar para las funciones de GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "316262299 - Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	//Establece las funciones de CallBack requeridas
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// Establece esto en true para que GLEW sepa usar un enfoque moderno para recuperar punteros de función y extensiones
	glewExperimental = GL_TRUE;

	// Inicializa a GLWE para configurar los punteros de la función OpenGL 
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define las dimensiones del viewport
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	// Crea funciones de tipo Shader
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");	//Carga del shader de ilumiación
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");				//Carga del shader lamp
	Shader AnimNavi("Shaders/anim.vs", "Shaders/anim.frag");				//Carga del shader de animación de NAVI
	
	// Definición y carga de modelos
	//---------------------------------------------------MUEBLES-----------|
	//Se crean objetos de tipo MODEL que permite cargar los modelos que se crearon en Maya y traerlos a OpenGL
	Model Cajonera((char*)"Models/CasaCompleta/Cajonera.obj");		//Se define el modelo de la CAJONERA
	Model Cajon((char*)"Models/CasaCompleta/Cajon.obj");			//Se define el modelo del CAJÓN
	Model Cama1Base((char*)"Models/CasaCompleta/Cama1Base.obj");	//Se define el modelo de la base de la CAMA 1
	Model Cama1Cama((char*)"Models/CasaCompleta/Cama1Cama.obj");	//Se define el modelo de el cochon y almohada de la CAMA 1
	Model Cama2Base((char*)"Models/CasaCompleta/Cama2Base.obj");	//Se define el modelo de la base de la CAMA 2
	Model Cama2Cama((char*)"Models/CasaCompleta/Cama2Cama.obj");	//Se define el modelo de el cochon y almohada de la CAMA 2
	Model Buffetero((char*)"Models/CasaCompleta/Buffetero.obj");	//Se define el modelo del BUFETERO
	Model Mesa((char*)"Models/CasaCompleta/Mesa.obj");				//Se define el modelo de la MESA
	Model Silla1Base((char*)"Models/CasaCompleta/Silla1Base.obj");	//Se define el modelo de la base de la SILLA 1
	Model Silla1Silla((char*)"Models/CasaCompleta/Silla1Silla.obj");//Se define el modelo de la tela de la SILLA 1
	Model Silla2Base((char*)"Models/CasaCompleta/Silla2Base.obj");	//Se define el modelo de la base de la SILLA 2
	Model Silla2Silla((char*)"Models/CasaCompleta/Silla2Silla.obj");//Se define el modelo de la tela de la SILLA 2
	Model Vaso1((char*)"Models/CasaCompleta/Vaso1.obj");			//Se define el modelo del VASO 1
	Model Vaso2((char*)"Models/CasaCompleta/Vaso2.obj");			//Se define el modelo del VASO 2
	Model Jarron1((char*)"Models/CasaCompleta/Jarron1.obj");		//Se define el modelo del JARRÓN 1
	Model Jarron2((char*)"Models/CasaCompleta/Jarron2.obj");		//Se define el modelo del JARRÓN 2

	Model Rupia((char*)"Models/Rupia/Rupia.obj");					//Se define el modelo de la RUPIA

	Model Navi((char*)"Models/Navi/Navi.obj");						//Se define el modelo de NAVI		
	Model Ala1((char*)"Models/Navi/Ala1.obj");						//Se define el modelo de al ALA superior derecha
	Model Ala2((char*)"Models/Navi/Ala2.obj");						//Se define el modelo de al ALA superior izquierda
	Model Ala3((char*)"Models/Navi/Ala3.obj");						//Se define el modelo de al ALA inferior derecha
	Model Ala4((char*)"Models/Navi/Ala4.obj");						//Se define el modelo de al ALA inferior izquierda

	//--------------------------------------------------FACHADA-------------|
	Model Piso1((char*)"Models/CasaCompleta/Piso1.obj");			//Se define el modelo del PISO (MADERA) 
	Model Piso2((char*)"Models/CasaCompleta/Piso2.obj");			//Se define el modelo del PISO (MARCO DE PIEDRA)
	Model Piso3((char*)"Models/CasaCompleta/Piso3.obj");			//Se define el modelo del PISO (RAMPA DE METAL)
	Model Alfombra((char*)"Models/CasaCompleta/Alfombra.obj");		//Se define el modelo de la ALFOMBRA
	Model Muros1((char*)"Models/CasaCompleta/Muros1.obj");			//Se define el modelo de los MUROS (LATERALES Y TRASERO)
	Model Muros2((char*)"Models/CasaCompleta/Muros2.obj");			//Se define el modelo de los MUROS (FRENTE)
	Model Techo((char*)"Models/CasaCompleta/Techo.obj");			//Se define el modelo del TECHO

	// Se configura el VAO del contenedor así como del VBO
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Atributos de posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Atributos de la normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Configuración de las unidades de textura
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"),1);


	//Configuración de la proyección, en este caso, se utiliza proyección perspectiva
	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop: Ciclo While que se donde se cofigura y establecen atributos de las luz y modelos, esto se repetirá hasta que
	//la ventana sea cerrada por el usuario
	while (!glfwWindowShouldClose(window))
	{

		// Calcula el DeltaTime del frame actual
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Revisa si se ha activado algún evento, ya sea pulsación de tecla o movimiento de ratón, 
		// esto abarca también las animaciones que sigan en un loop (RupiaAnim y AlasAnim)
		glfwPollEvents();
		DoMovement();
		RupiaAnim();
		AlasAnim();

		// Limpia el ColorBuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
		// Opciones de OpenGL
		glEnable(GL_DEPTH_TEST);

		// Use cooresponding shader when setting uniforms/drawing objects
		// Usa el shader correspondiente al configurar dibujar uniforms/objetos
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.35f, 0.35f, 0.35f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);


		// Point light
		//Se define y se define el color de la PointLight
	    glm::vec3 lightColor;	
		lightColor.x= 0.0f;
		lightColor.y= 1.0f;
		lightColor.z= 1.0;

		//Configuración de Point Light. Donde se configura la posición, la componente ambiental, difusa, especular,
		// constante (no se modifica), lineal y cuadratica
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x,lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x,lightColor.y,lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),1.8f);



		// Configuración de SpotLight. Donde se configura la posición, dirección, la componente ambiental, difusa, especular,
		// constante (no se modifica), lineal, cuadratica, el angulo cutOff y el angulo outerCutOff 
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), 0.0f, 3.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, LuzDirY, -0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(40.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(42.0f)));

		// Configuración de propiedades de materiales
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 10.0f);

		// Crea transformaciones de la cámara
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Obtiene las ubicaciones de las uniformes
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pasa las matrices al shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//Carga de modelo
		glm::mat4 model(1);				
        view = camera.GetViewMatrix();	//Se agina la matriz de vista

		//La mayoría de las posiciones, escalas y rotaciones los objetos, como los muebles y la fachada 
		// ya fueron establecidos desde Maya, por lo que no será necesario hacer dichas transformaciones básicas
		////--------------------------------------------------------------------Cajonera ---------------|
		model = glm::mat4(1);	//Se asigna la matriz de modelo para que se puedan modificar sus atributos 								
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0); //Se indica si se quiere o no activar la transparencia
																							 //en caso de requerirlo
		Cajonera.Draw(lightingShader); //Se dibuja el Cuerpo de la cajonera
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f); //Se indica el nivel de transparencia.

		model = glm::mat4(1);	//Se setea la matriz de modelo para que se pueda trabajar con el siguiente modelo
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, CajonMov));//Se traslada en Z con la varibale CajonMov para la animación
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cajon.Draw(lightingShader);		//Se dibuja el Cajón
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 1.0f);

		////--------------------------------------------------------------------Cama 1 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cama1Base.Draw(lightingShader);	//Se dibuja la Base de la Cama 1
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cama1Cama.Draw(lightingShader);	//Se dibuja el Colchón y almohada de la Cama 1
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Cama 2 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cama2Base.Draw(lightingShader);	//Se dibuja la Base de la Cama 2
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Cama2Cama.Draw(lightingShader);	//Se dibuja el Colchón y almohada de la Cama 2
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Buffetero ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Buffetero.Draw(lightingShader);	//Se dibuja el Buffetero
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Mesa ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Mesa.Draw(lightingShader);		//Se dibuja la Mesa
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Silla 1 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Silla1Base.Draw(lightingShader);//Se dibuja la base de la Silla 1
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Silla1Silla.Draw(lightingShader);//Se dibuja la tela de la Silla 1
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Silla 2 ---------------|
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, SillaMov));//Se traslada en Z con la varibale SillaMov para la animación
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Silla2Base.Draw(lightingShader);//Se dibuja la base de la Silla 2
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		model = glm::mat4(1); 
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, SillaMov));//Se traslada en Z con la varibale SillaMov para la animación
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Silla2Silla.Draw(lightingShader);//Se dibuja la tela de la Silla 2
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Vaso 1 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Vaso1.Draw(lightingShader);		//Se dibuja el Vaso 1
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Vaso 2 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Vaso2.Draw(lightingShader);		//Se dibuja el Vaso 2
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Jarron 1 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Jarron1.Draw(lightingShader);	//Se dibuja el Jarrón 1
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Jarron 2 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Jarron2.Draw(lightingShader);	//Se dibuja el Jarrón 2
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Piso 1 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Piso1.Draw(lightingShader);		//Se dibuja la primera parte del piso (Madera)
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Piso 2 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Piso2.Draw(lightingShader);		//Se dibuja la segunda parte del piso (marco de piedra)
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Piso 3 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Piso3.Draw(lightingShader);		//Se dibuja la tercera parte del piso (rampa de metal)
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Alfombra ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Alfombra.Draw(lightingShader);	//Se dibuja la Alfombra
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Muros 1 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Muros1.Draw(lightingShader);	//Se dibuja los muros laterales y trasero
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Muros 2 ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Muros2.Draw(lightingShader);	//Se dibuja el muro de frente
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Techo ---------------|
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Techo.Draw(lightingShader);		//Se dibuja el techo
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////-------------------------------DIBUJADO DE OBJETOS PARA ANIMACIONES COMPLEJAS-----------------------|
		////--------------------------------------------------------------------Rupia ---------------|
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.0f, 0.5f, 3.0f));			     //Se establece la posición de la rupia
		model = glm::translate(model, glm::vec3(RupiaMovX, RupiaMovY, RupiaMovZ));   //Se traslada la Rupia para la animación dependiendo de
																				     //RupiaMovX, RupiaMovY, RupiaMovZ
		model = glm::rotate(model, glm::radians(rotKit), glm::vec3(0.0f, 1.0f, 0.0));//La rupia rota según rotKit para la animación
		model = glm::scale(model, glm::vec3(0.2f));									 //Se hace pequeña la rupia
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Rupia.Draw(lightingShader);		//Se dibuja Rupia
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		////--------------------------------------------------------------------Navi ---------------|
		AnimNavi.Use();	// Se llama al método AnimNavi que contiene los calculos de animación en el shader actual
						// Esto abarcará desde el cuerpo de Navi hasta sus alas

		tiempo = glfwGetTime();	// Se usa el temporizador de GLFW para una variable donde va aumentando automaticamente desde que
								// se inicializa GLFW
		
		//Al tratarse de un shader distinto, se obtienen nuevamente las ubicaciones de las uniformes
		modelLoc = glGetUniformLocation(AnimNavi.Program, "model");
		viewLoc = glGetUniformLocation(AnimNavi.Program, "view");
		projLoc = glGetUniformLocation(AnimNavi.Program, "projection");
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));	//Se establece la posición de Navi
		model = glm::scale(model, glm::vec3(0.1f));					//Se escala en 0.1 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(AnimNavi.Program, "time"), tiempo); //Se manda la variable tiempo a anim.vs para obtener 
																			 //el efecto de moviemiento con una función seno (sin)
		Navi.Draw(lampShader);			//Se dibuja Navi 

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(AlasRotIzq), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(AnimNavi.Program, "time"), tiempo);
		Ala1.Draw(lampShader);			//Se dibuja la ala izquierda superior

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(AlasRotDer), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(AnimNavi.Program, "time"), tiempo);
		Ala2.Draw(lampShader);			//Se dibuja la ala derecha superior

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(AlasRotIzq), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(AnimNavi.Program, "time"), tiempo);
		Ala3.Draw(lampShader);			//Se dibuja la ala izquierda inferior

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		model = glm::rotate(model, glm::radians(AlasRotDer), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.1f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(AnimNavi.Program, "time"), tiempo);
		Ala4.Draw(lampShader);			//Se dibuja la ala derecha inferior

		glBindVertexArray(0);
	
		// Se dibuja nuevamente el objeto lamp para el shader apropiado para el PointLight
		lampShader.Use();

		// Obtiene objetos de ubicación para las matrices en el lamp shader 
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Configura las matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// Dibuja el obejto de luz (utilizando atributos de las vertices de luz
		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// Intercambiar los búferes de pantalla
		glfwSwapBuffers(window);
	}

	// Se termina GLFW, limpiando cualquier recurso utilizado por GLFW.
	glfwTerminate();

	return 0;
}

// Función que permite mueve la posición de la cámara según las teclas que precione el usuario
void DoMovement()
{

	// Cotroles de la cámara
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])		//Al presionar las tecla 'W' o flecha de 'Arriba' se procesará para
	{												//que la cámara vaya hacia adelante
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])	//Al presionar las tecla 'S' o flecha de 'Abajo' se procesará para
	{												//que la cámara vaya hacia atrás
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])	//Al presionar las tecla 'A' o flecha de 'izquierda' se procesará para
	{												//que la cámara vaya hacia la izquierda
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])	//Al presionar las tecla 'D' o flecha de 'derecha' se procesará para
	{												//que la cámara vaya hacia la derecha
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	//-------------------------------------SILLA--------|
	//Incrementa y decrementa la varible SillaMov para la traslación de la Silla
	if (SillaAnim) {
		if (SillaMov >= -1.0f) {
			SillaMov -= 0.05f;
		}
	}
	else {
		if (SillaMov <= 0.0f) {
			SillaMov += 0.05f;
		}
	}

	//-----------------------------------CAJON--------|
	//Incrementa y decrementa la varible CajonMov para la traslación de la Silla
	if (CajonAnim) {
		if (CajonMov <= 0.5f) {
			CajonMov += 0.05f;
		}
	}
	else {
		if (CajonMov >= 0.0f) {
			CajonMov -= 0.05f;
		}
	}

	//-----------------------------------RUPIA--------|
	//Se restablecen los valores para los recorridos de la animación de la rupia al presionar 'H'
	if (keys[GLFW_KEY_H]) {
		RupiaCir = true;
		RupiaRec1 = true;
		RupiaRec2 = false;
		RupiaRec3 = false;
		RupiaRec4 = false;
		RupiaMovY = 0.0f;
		RupiaMovX = 0.0f;
		RupiaMovX2 = 0.0f;
		RupiaMovZ = 0.0f;
	}
}

// Función que contiene las variables y calculos para realizar la animación de la rupia (parabolas) 
void RupiaAnim() {
	
	// Recorridos para a seguir para animación de rebote de rupia
	if (RupiaCir) {			//Cuando se presione la tecla 'H' se activa/desactiva la animación
		rotKit += 45.0f;	//La variable se encarga de rotar 45 grados de manera infinita
		
		//La rupia se traslada hasta que llegue al valor de 2.0f en Y
		if (RupiaRec1) {
			RupiaMovY += 0.05f;
			if (RupiaMovY > 2.0f) {
				RupiaRec1 = false;
				RupiaRec2 = true;
			}
		}

		//Se calcula por medio de una parabola la animación de rebote. Donde RupiaMovY depende de RupiaMovX incrementando
		//ambas variables
		//Parabola: f(x) = 0.05 - x^2  
		if (RupiaRec2) {
			RupiaMovX += 0.05f;
			RupiaMovZ -= 0.05f;
			RupiaMovY += 0.05f - (RupiaMovX * RupiaMovX);	//La función se suma así misma para darle el efecto de rebote
			if (RupiaMovY <= 0.0f) {
				RupiaMovY = 0.0f;
				RupiaRec2 = false;
				RupiaRec3 = true;
			}
		}

		//Se calcula por medio de una parabola la animación del segundo rebote. Se usa RupiaMovX2 como referencia 
		//para el calculo de la parabola, pero sin afectar RupiaMovX de su posición  
		//Parabola: g(x) = 0.1f - x^2
		if (RupiaRec3) {
			RupiaMovX += 0.05f;
			RupiaMovX2 += 0.05f;
			RupiaMovZ -= 0.05f;
			RupiaMovY += 0.1f - (RupiaMovX2 * RupiaMovX2); //La función se suma así misma para darle el efecto de rebote
			if (RupiaMovY <= 0.0f) {
				RupiaMovY = 0.0f;
				RupiaRec3 = false;
				RupiaRec4 = true;
			}
		}

		//Deja la rupia girando hasta que se vuelva a presionar la tecla 'H'
		if (RupiaRec4) {
			RupiaRec4 = true;
		}
	}
}

// Función que contiene las variables y calculos para realizar la animación de las alas de Navi (rotación de alas)
void AlasAnim() {

	//En la animación de las alas de Navi no se tiene que activar. Se incrementa AlasRotDer y se decrementa AlasRotIzq
	//para rotar los objetos de las alas hacia 'dentro'. Esto se detiene hasta que se rotan 30.0°
	if (AlasRot) {
		AlasRotIzq -= 1.0f;	
		AlasRotDer += 1.0f;
		if (AlasRotIzq == -30.0f)
			AlasRot = false;
	}

	//Se decrementa AlasRotDer y se incrementa AlasRotIzq para rotar los objetos de las alas hacia 'afuera'. 
	//Esto se detiene hasta que se rotan 0°
	else {
		AlasRotIzq += 1.0f;
		AlasRotDer -= 1.0f;
		if (AlasRotIzq == 0.0f)
			AlasRot = true;
	}
}

// Es llamado cada vez que se presiona/libera una tecla por medio de GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	// Cierra la ventana cuando se preciona la teclas 'Escape'
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	//-----------------------------------LUZ---------|
	//Se activa/desactiva la animación de luz cada vez que se presiona la tecla 'T'. Mueve la dirección de la luz de
	//-1.0 en dirección de Y a 0.0 y visecersa
	if (keys[GLFW_KEY_T]) {
		LuzAnim = !LuzAnim;
		if (LuzAnim) {
			LuzDirY = -1.0f; 
		}
		else {
			LuzDirY = 0.0f;
		}
	}
	//-----------------------------------Silla--------|
	//Se activa/desactiva la animación de la silla cada vez que se presiona la tela 'G'. Se activa y desactiva cuando
	//SillaMov >= -1.0 y viceversa
	if (keys[GLFW_KEY_G]) {
		if (SillaMov >= -1.0f) {
			SillaAnim = true;
		}
		else {
			SillaAnim = false;

		}
	}
	//-----------------------------------Cajon--------|
	//Se activa/desactiva la animación del cajón cada vez que se presiona la tela 'Y'. Se activa y desactiva cuando
	//CajonMov <= 0.5 y viceversa
	if (keys[GLFW_KEY_Y]) {
		if (CajonMov <= 0.5f) {
			CajonAnim = true;
		}
		else {
			CajonAnim = false;

		}
	}
}

//Función que la posición del cursor del ratón para el movimiento de la cámara
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  //Invertida ya que las coordenadas y van de abajo hacia la izquierda

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}