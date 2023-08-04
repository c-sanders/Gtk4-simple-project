#include <iostream>
#include <string>
#include <typeinfo>

#include <gtk/gtk.h>
// Function(s) : gtk_application_new
#include <gtk/gtktypes.h>
// Type(s) : _GtkBuilder

#include <FileUtils.hpp>
// #include <CraigConfig.hpp>


using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ostringstream;


// Declare global variables.

bool         read_UI_directly_from_file = false;

const
string       nameUIFile = "c:\\users\\craig\\Documents\\GitHub\\Gtk4-simple-project\\xml\\ui\\simple.ui";

int          status = -1;

string       xmlString;

GtkBox     * widget_p;

GSList     * list_p;

GtkBuilder * builder_p = NULL;

GtkWidget  * window_p,
           * button_quit_p;


/* GtkBuilder is defined as follows, in the file gtktypes.h;
 * 
 * typedef struct _GtkBuilder             GtkBuilder; 
 */

/* _GtkBuilder is defined as follows, in the file gtkbuilder.h;
 * 
 * struct _GtkBuilder
 * {
 *  GObject parent_instance;
 *
 *  GtkBuilderPrivate *priv;
 * }; 
 */

/*
struct _GtkBuilder
{
 GObject             parent_instance;
 GtkBuilderPrivate * priv;
};
 */


void
clicked_quitButton
(
 GtkApplication * app,
 gpointer         user_data
)
{
    const
    string   nameFunction = "clicked_quitButton",
             nF = nameFunction + " : ";


    cout << nF << "Enter" << endl;

    cout << nF << "Exit" << endl;
}


void
functionHandler
(
 gpointer   data,
 gpointer   user_data
)
{
    const
    string   nameFunction = "functionHandler",
             nF = nameFunction + " : ";


    cout << nF << "Enter" << endl;

    cout << nF << "Exit" << endl;
}


void
createWindow
(
)
{
    const
    string   nameFunction = "createWindow",
             nF = nameFunction + " : ";


    cout << nF << "Enter" << endl;

    if (read_UI_directly_from_file)
    {
        // Create the builder by reading its description directly from a UI file.

        builder_p = gtk_builder_new_from_file
        (
            nameUIFile.c_str()
        );
    }
    else
    {
        // Read the contents of the UI file into a string.

        readWholeFileIntoString
        (
            nameUIFile,
            &xmlString
        );

        cout << xmlString << endl;

        // Create a Gtk Builder object from the UI which is described in the string.

        builder_p = gtk_builder_new_from_string
        (
            xmlString.c_str(),
            xmlString.length()
        );
    }

    // Test the Gtk Builder object.

    if (
        GTK_IS_BUILDER(builder_p) &&
        (builder_p != NULL)
        )
    {
        cout << nF << "Is of type GtkBuilder" << endl;
    }
    else
    {
        cout << nF << "Seem to have encountered an error constructing an object of class GtkBuilder" << endl;

        status = -1;

        goto return_error_routine;
    }

    // widget_p = builder_1_p->get_object("window1");

    widget_p = GTK_BOX
    (
        gtk_builder_get_object
        (
            builder_p,
            "top_box_layout"
        )
    );

    button_quit_p = GTK_WIDGET
    (
        gtk_builder_get_object
        (
            builder_p,
            "button_quit"
        )
    );

    cout << nF << "Typeid of widget_p = " << typeid(widget_p).name() << endl;
    cout << nF << "Typeid of button_quit_p = " << typeid(button_quit_p).name() << endl;

    list_p = gtk_builder_get_objects(builder_p);

    cout << nF << "Number of elements in list = " << g_slist_length(list_p) << endl;

    goto return_routine;


return_error_routine :


return_routine :

    cout << nF << "Enter" << endl;
}

static
void
activate
(
 GtkApplication * app,
 gpointer         user_data
)
{
    const
    string       nameFunction = "main",
                 nF = nameFunction + " : ";


    cout << nF << "Enter" << endl;

    createWindow();

    window_p = gtk_application_window_new(app);
    
    gtk_window_set_title(GTK_WINDOW(window_p), "Window");
    
    gtk_window_set_default_size(GTK_WINDOW(window_p), 200, 200);
    
    // Populate the window with widgets from the UI file.

    gtk_window_set_child(GTK_WINDOW(window_p), GTK_WIDGET(widget_p));

    // Connect Events to Event handlers.

    g_signal_connect
    (
     button_quit_p,
     "clicked",
     G_CALLBACK(clicked_quitButton),
     NULL
    );

    // Display the window now that it has been populated with widgets.

    gtk_widget_set_visible(window_p, true);

    goto return_routine;


return_error_routine :


return_routine :

    cout << nF << "Status = " << status << endl;

    cout << nF << "Exit" << endl;
}


int
main
(
 int      argc,
 char * * argv
)
{
    const
    string           nameFunction = "main",
                     nF           = nameFunction + " : ";

    int              status = 0;

    GtkApplication* app = NULL;


    cout << nF << "Enter" << endl;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);

    cout << nF << "Exit" << endl;


    return status;
}