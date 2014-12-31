/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("NOT READY YET DON'T LOOK AT THIS EXAMPLE!!!!", Ness::Sizei(800, 600)); //, Ness::DEFAULT_WINDOW_FLAGS_FULLSCREEN);
	renderer.set_background_color(Ness::Color(0.1f, 0.2f, 0.2f));

	// create a new gui manager
	Ness::Gui::GuiManagerPtr gui = renderer.create_gui_manager("../ness-engine/resources/gui/");
	//Ness::Gui::ContainerPtr root_container = gui->create_container();

	// create forms in all docking areas
	Ness::Gui::ContainerPtr bottom_left = gui->create_container(Ness::Point(4, 4));
	bottom_left->create_label("hello world!");
	bottom_left->dock_to(Ness::Gui::DOCK_BOTTOM_LEFT);

	Ness::Gui::ContainerPtr bottom_right = gui->create_container(Ness::Point(4, 4));
	bottom_right->dock_to(Ness::Gui::DOCK_BOTTOM_RIGHT);

	Ness::Gui::ContainerPtr bottom_center = gui->create_container(Ness::Point(4, 4));
	bottom_center->dock_to(Ness::Gui::DOCK_BOTTOM_CENTER);

	Ness::Gui::ContainerPtr left_center = gui->create_container(Ness::Point(4, 4));
	left_center->dock_to(Ness::Gui::DOCK_LEFT_CENTER);

	Ness::Gui::ContainerPtr right_center = gui->create_container(Ness::Point(4, 4));
	right_center->dock_to(Ness::Gui::DOCK_RIGHT_CENTER);

	Ness::Gui::ContainerPtr top_left = gui->create_container(Ness::Point(4, 4));
	top_left->dock_to(Ness::Gui::DOCK_TOP_LEFT);

	Ness::Gui::ContainerPtr top_right = gui->create_container(Ness::Point(4, 4));
	top_right->dock_to(Ness::Gui::DOCK_TOP_RIGHT);

	Ness::Gui::ContainerPtr top_center = gui->create_container(Ness::Point(4, 4));
	top_center->dock_to(Ness::Gui::DOCK_TOP_CENTER);

	Ness::Gui::ContainerPtr center = gui->create_container(Ness::Point(12, 7));
	center->dock_to(Ness::Gui::DOCK_CENTER);

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(*gui.get());
	EventsPoller.add_handler(app);

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// render the scene
		renderer.start_frame();
		gui->render();
		renderer.end_frame();
	}

	// finish
	return 0;
}