#include <string>
#include <iostream>

#include <gtk/gtk.h>


using std::string;
using std::cout;
using std::endl;


const
string               nameUIFile_0 = "C:\\Users\\craig\\Documents\\GitHub\\Gtk4-simple-project\\xml\\ui\\BasicLayout.ui",
                     nameUIFile_1 = "C:\\Users\\craig\\Documents\\GitHub\\Gtk4-simple-project\\xml\\ui\\Button_DrawingArea_Button.ui",
                     nameUIFile   = nameUIFile_1;

GtkWidget          * window_p;

GtkEventController * gesture_left_mouse_button_click_EventController_p,
                   * gesture_right_mouse_button_click_EventController_p;


// Forward declarations of functions which are defined at the bottom of this
// file.

static
void
activate
(
    GtkApplication* app,
    gpointer         user_data
);


int
createAndConfigureMouseButtonEventController
(
 GtkEventController * eventController_p,
 int                  mouseButtonNumber,
 const
 string               gestureDescription,
 GtkWidget          * associatedWidget_p,
 const
 string               eventType
);


static
void
slot_mouseButtonAction
(
 GtkApplication * app,
 gpointer         user_data
);


static
void
slot_shutdown
(
 GtkApplication * app,
 gpointer         user_data
);


int
main
(
 int      argc,
 char * * argv
)
{
    GtkApplication * app;

    int              status;


    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect
    (
     app,
     "activate",
     G_CALLBACK(activate),
     NULL
    );

    // The following actually works, although it generates runtime warnings.
    // Why however, does it generate these runtime warnings?
  
    /*
    g_signal_connect
    (
     window,
     "destroy",
     G_CALLBACK(slot_shutdown),
     NULL
    );
     */

    status = g_application_run
             (
              G_APPLICATION(app),
              argc,
              argv
             );

    g_object_unref (app);


    return status;
}


static
void
activate
(
 GtkApplication * app,
 gpointer         user_data
)
{
    GtkWidget  * widget_boxLayout_top_p,
               * button_p,
               * drawingArea_p;

    GtkBuilder * builder_p;


    window_p = gtk_application_window_new(app);

    // Open a UI file and read its contents into the Builder object.

    builder_p = gtk_builder_new_from_file(nameUIFile.c_str());

    // Associate Widget pointers with the relevant Builder objects which were
    // loaded from the UI file. 

    widget_boxLayout_top_p = GTK_WIDGET
                             (
                              gtk_builder_get_object
                              (
                               const_cast<GtkBuilder*>(builder_p),
                               "box_layout_vertical"
                              )
                             );

    button_p = GTK_WIDGET
               (
                gtk_builder_get_object
                (
                 const_cast<GtkBuilder*>(builder_p),
                 "button_1"
                )
               );

    drawingArea_p = GTK_WIDGET
                    (
                     gtk_builder_get_object
                     (
                      const_cast<GtkBuilder*>(builder_p),
                      "drawingArea"
                     )
                    );

    gtk_window_set_title
    (
     GTK_WINDOW(window_p),
     "Window"
    );

    gtk_window_set_default_size
    (
     GTK_WINDOW(window_p),
     200,
     200
    );

    // Create a Controller and associate it with the Button widget.
    //
    // This Controller should handle any right mouse button clicks which are
    // performed on the Button widget.

    gesture_right_mouse_button_click_EventController_p = GTK_EVENT_CONTROLLER
                                                         (
                                                          gtk_gesture_click_new()
                                                         );

    createAndConfigureMouseButtonEventController
    (
     gesture_right_mouse_button_click_EventController_p,
     3,
     "Event Controller : Right Mouse Button clicked",
     drawingArea_p,
     "pressed"
    );

    // Create another Controller and also associate it with the Button widget.
    //
    // This Controller should handle any left mouse button clicks which are
    // performed on the Button widget.

    gesture_left_mouse_button_click_EventController_p = GTK_EVENT_CONTROLLER
                                                        (
                                                         gtk_gesture_click_new()
                                                        );

    createAndConfigureMouseButtonEventController
    (
     gesture_left_mouse_button_click_EventController_p,
     1,
     "Event Controller : Left Mouse Button clicked",
     drawingArea_p,
     "pressed"
    );

    g_signal_connect
    (
     window_p,
     "destroy",
     G_CALLBACK(slot_shutdown),
     NULL
    );

    // Set the main widget for the window. 

    gtk_window_set_child
    (
     GTK_WINDOW(window_p),
     GTK_WIDGET(widget_boxLayout_top_p)
    );

    // Does gtk_widget_set_visible need to be invoked at this point? 

    gtk_widget_show(window_p);
}


// -------------------------------------------------------------------------
// Can multiple gestures be associated with the same Widget?
// 
// For example, is it possible to associate a left mouse button click
// gesture and a right mouse button click gesture with the same widget? 
// -------------------------------------------------------------------------

// What type of Controller should be created?
//
//   - GtkEventControllerKey
//   - GtkPadController
//
// Or doesn't it even matter? How about GtkGestureSingle?
//
// Refer to;
//
//   https://discourse.gnome.org/t/how-to-claim-right-click-gestures-released-event-in-gtk4/4194/4

int
createAndConfigureMouseButtonEventController
(
 GtkEventController * eventController_p,
 int                  mouseButtonNumber,
 const
 string               gestureDescription,
 GtkWidget          * associatedWidget_p,
 const
 string               eventType
)
{
    const
    string   nameFunction = "createAndConfigureMouseButtonEventController",
             nF           = nameFunction + " : ";


    // Instruct the Event Controller to only capture right mouse button clicks.
    // The number 3 denotes the right mouse button.

    cout << nF << "About to associate a button with the Event Controller" << endl;

    gtk_gesture_single_set_button
    (
     GTK_GESTURE_SINGLE(eventController_p),
     mouseButtonNumber
    );

    cout << nF << "Have associated a button with the Event Controller" << endl;

    gtk_event_controller_set_name
    (
     eventController_p,
     gestureDescription.c_str()
    );

    gtk_widget_add_controller
    (
     associatedWidget_p,
     eventController_p
    );

    // Can use   : pressed, released
    // Can't use : clicked

    g_signal_connect
    (
     eventController_p,
     eventType.c_str(),
     G_CALLBACK(slot_mouseButtonAction),
     NULL
    );

    return 0;
}


static
void
slot_mouseButtonAction
(
 GtkApplication * app,
 gpointer         user_data  // Does this value represent the number which
                             // corresponds to the mouse button which has been
                             // clicked, or does it represent the number of
                             // clicks which have been performed in a particular
                             // amount of time?
)
{
    const
    string         nameFunction = "slot_mouseButtonAction",
                   nF           = nameFunction + " : ";

    GtkWidget    * widget_p     = NULL;

    GdkDevice    * device_p     = NULL;

    GdkEvent     * event_p      = NULL;

    GdkEventType   eventType;


    // Parameter user_data receives a value of 1. Does this denote that the left
    // button has been clicked?

    cout << nF << "Enter" << endl;

    widget_p = gtk_event_controller_get_widget(gesture_right_mouse_button_click_EventController_p);
    device_p = gtk_event_controller_get_current_event_device(gesture_right_mouse_button_click_EventController_p);
    event_p  = gtk_event_controller_get_current_event(gesture_right_mouse_button_click_EventController_p);

    // Gdk Event types. Refer to https://docs.gtk.org/gdk4/enum.EventType.html

    eventType = gdk_event_get_event_type(event_p);

    // Reset the Controller, otherwise it gets stuck in a particular state and
    // won't handle anymore mouse button clicks.

    gtk_event_controller_reset(gesture_right_mouse_button_click_EventController_p);

    cout << nF << "Address of Event Controller object = " << widget_p << endl;
    cout << nF << "Value passed to parameter user_data = " << user_data << endl;

    cout << nF << "Name of Controller widget = " << gtk_event_controller_get_name(gesture_right_mouse_button_click_EventController_p) << endl;

    cout << nF << "Name of device = " << gdk_device_get_name(device_p) << endl;
    cout << nF << "Event type = " << eventType << endl;
    cout << nF << "Exit" << endl;
}


static
void
slot_shutdown
(
 GtkApplication * app,
 gpointer         user_data
)
{
    const
    string   nameFunction = "slot_shutdown",
             nF           = nameFunction + " : ";


    cout << nF << "Enter" << endl;

    cout << nF << "Exit" << endl;
}
