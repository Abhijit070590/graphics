#include <string.h>
#include <GL/glui.h>
#include "../Headers/Mesh.h"
float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;
int   main_window;


/** These are the live variables passed into GLUI ***/
int   wireframe = 0;
int   obj = 0;
int   segments = 8;

Mesh Sub_mesh_d;
Mesh Con_mesh_d;
Mesh Sub_mesh_e;
Mesh Con_mesh_e;


// Using a char buffer as a live var is also possible, but it is dangerous
// because GLUI doesn't know how big your buffer is.
// But still, it works as long as text doesn't happen to overflow.
//char  text[200] = {"Hello World!"};

GLUI_Checkbox   *checkbox;
GLUI_Spinner    *spinner;
GLUI_RadioGroup *radio;
GLUI_Rotation 	*Light_rotate;
GLUI_Rotation   *rotate;
float view_rotate[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
float light_rotate[16] ={1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};
/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb( int control )
{
  /********************************************************************
    Here we'll print the user id of the control that generated the
    callback, and we'll also explicitly get the values of each control.
    Note that we really didn't have to explicitly get the values, since
    they are already all contained within the live variables:
    'wireframe',  'segments',  'obj',  and 'text'
    ********************************************************************/

  printf( "callback: %d\n", control );
  printf( "             checkbox: %d\n", checkbox->get_int_val() );
  printf( "              spinner: %d\n", spinner->get_int_val() );
  printf( "          radio group: %d\n", radio->get_int_val() );
  //printf( "                 text: %s\n", edittext->get_text() );

}

/***************************************** myGlutIdle() ***********/

void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window )
    glutSetWindow(main_window);

  glutPostRedisplay();
}


/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
    // A few keys here to test the sync_live capability.
  case 'o':
    // Cycle through object types
    ++obj %= 3;
    GLUI_Master.sync_live_all();
    break;
  case 'w':
    // Toggle wireframe mode
    wireframe = !wireframe;
    GLUI_Master.sync_live_all();
    break;
  case 27:
  case 'q':
    exit(0);
    break;
  };
  glutPostRedisplay();
}


/***************************************** myGlutMenu() ***********/

void myGlutMenu( int value )
{
  myGlutKeyboard( value, 0, 0 );
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y )
{
  if ( button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) {
    last_x = x;
    last_y = y;
  }
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y )
{
  rotationX += (float) (y - last_y);
  rotationY += (float) (x - last_x);

  last_x = x;
  last_y = y;

  glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
  xy_aspect = (float)x / (float)y;
  glViewport( 0, 0, x, y );

  glutPostRedisplay();
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0 );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
    glMultMatrixf( light_rotate );
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  glLoadIdentity();

  glTranslatef( 0.0f, 0.0f, -1.6f );
  glMultMatrixf(view_rotate);

  //glRotatef( rotationY, 0.0, 1.0, 0.0 );
  //glRotatef( rotationX, 1.0, 0.0, 0.0 );

  /*** Now we render object, using the variables 'obj', 'segments', and
    'wireframe'.  These are _live_ variables, which are transparently
    updated by GLUI ***/

  if ( obj == 0 ) {
    if ( wireframe )
    	Con_mesh_d.draw();
    else
    	Con_mesh_e.draw();
  }
  else if ( obj == 1 ) {
    if ( wireframe )
      Con_mesh_e.draw();
    else
      Sub_mesh_d.draw();

  }


  glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glColor3ub( 0, 0, 0 );
  glRasterPos2i( 10, 10 );

  //  printf( "text: %s\n", text );

  /*** Render the live character array 'text' ***/

  glEnable( GL_LIGHTING );

  glutSwapBuffers();
}


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	Sub_mesh_d.load_mesh(argv[1]);
	Con_mesh_d.load_mesh(argv[1]);
	Sub_mesh_d.triangulation();
  for(int i=0;i<3;i++){
	Sub_mesh_d.make_edges();
	Sub_mesh_d.make_neighbours();
	//Sub_mesh_d.calculate_normals();

	Sub_mesh_d.subdivide();
	Sub_mesh_d.calculate_normals();
}
	printf("\n\t\t Dodecahedron finished**********\n");
	Sub_mesh_e.load_mesh(argv[2]);
	Con_mesh_e.load_mesh(argv[2]);
	Sub_mesh_e.triangulation();
  Sub_mesh_e.calculate_normals();

	Sub_mesh_e.make_edges();
	Sub_mesh_e.make_neighbours();
	Sub_mesh_e.subdivide();
	Sub_mesh_e.calculate_normals();
  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/

  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize( 300, 300 );

  main_window = glutCreateWindow( "GLUI Example 2" );
  glutDisplayFunc( myGlutDisplay );
  glutReshapeFunc( myGlutReshape );
  glutKeyboardFunc( myGlutKeyboard );
  glutMotionFunc( myGlutMotion );
  glutMouseFunc( myGlutMouse );

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/

  GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
  GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
  GLfloat light0_position[] = {1.0f, 1.0f, 1.0f, 0.0f};

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);

  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/

  GLUI *glui = GLUI_Master.create_glui( "GLUI", 0, 400, 50 ); /* name, flags,
                 x, and y */
  new GLUI_StaticText( glui, "GLUI Example 2" );
  new GLUI_Separator( glui );
  checkbox = new GLUI_Checkbox( glui, "Control Mesh", &wireframe, 1, control_cb );

  spinner  = new GLUI_Spinner( glui, "Subdivision _factor", &segments, 2, control_cb );
  spinner->set_int_limits( 3, 60 );

  rotate = new GLUI_Rotation(glui, "view_rotate",view_rotate);
  rotate -> set_spin(1.0);

  Light_rotate = new GLUI_Rotation(glui, "light_rotate",light_rotate);
  Light_rotate -> set_spin(1.0);
  GLUI_Panel *obj_panel = new GLUI_Panel( glui, "mesh selection" );
  radio = new GLUI_RadioGroup( obj_panel,&obj,4,control_cb );
  new GLUI_RadioButton( radio, "Dodecahedron" );
  new GLUI_RadioButton( radio, "Ellel" );

  GLUI_Panel *control = new GLUI_Panel(glui,"Control Mesh Changer");
  new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );

  glui->set_main_gfx_window( main_window );

  /* We register the idle callback with GLUI, *not* with GLUT */
  GLUI_Master.set_glutIdleFunc( myGlutIdle );
  GLUI_Master.set_glutIdleFunc( NULL );

  glutMainLoop();

  return EXIT_SUCCESS;
}
