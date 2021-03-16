#include <gtk/gtk.h>
#include <stdlib.h>

int main( int argc, char* argv[]) {
	GtkBuilder *builder;
	GtkWidget *window;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "two_i.glade", NULL);

	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	gtk_builder_connect_signals(builder, NULL);

	g_object_unref(builder);

	gtk_widget_show(window);
	gtk_main();

	return 0;
}

#define START 0
#define CONTINUE 1
#define CANCEL -1
#define FAIL -2

char dialog_buffer[51];
int dialog_value = -1;
int val_min;
int val_max;

GtkWidget* create_dialog(const char* message) {
	GtkBuilder* builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "basic_dialog.glade", NULL);
	gtk_builder_connect_signals(builder, NULL);

	GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_main"));
	GtkWidget* label = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_message"));
	gtk_label_set_text(GTK_LABEL(label), message);

	gtk_widget_show(window);
	return window;
}

GtkWidget* create_num_entry(const char* message, int num_min, int num_max) {
	strcpy(dialog_buffer, "");
	char* formatted_message = malloc(strlen(message) + 50);
	val_min = num_min;
	val_max = num_max;

	GtkBuilder* builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "basic_num_dialog.glade", NULL);
	gtk_builder_connect_signals(builder, NULL);

	GtkWidget* window = GTK_WIDGET(gtk_builder_get_object(builder, "num_dialog_main"));
	GtkWidget* label = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_message"));
	sprintf(formatted_message, message, num_min, num_max);
	gtk_label_set_text(GTK_LABEL(label), formatted_message);

	gtk_widget_show(window);
	free(formatted_message);
	return window;
}

GtkWidget* wrap_up_stuff() { return NULL; }

void process(int new_state) {
	static int state;

	switch(new_state) {
	case START:
		state = 0; break;
	case CANCEL:
		state--; break;
	case FAIL:
		state = -1; return;
	case CONTINUE:
		state++; break;
	default:
		fprintf(stderr, "invalid process input %d\n", new_state);
		return;
	}

	static GtkWidget* window = NULL;
	switch(state) {
	case 0:
		if (window) gtk_widget_destroy(window);
		window = create_num_entry("Enter a number between %d and %d", 0, 100); break;
	case 1:
		gtk_widget_destroy(window);
		window = create_dialog("Congratulations"); break;
	case 2: //completed
		gtk_widget_destroy(window);
		window = wrap_up_stuff (); break;
	case -1:
		if (window) gtk_widget_destroy(window);
		break;
	default:
		fprintf(stderr, "unknown state reached\n");
	}
}

void on_dialog_submit_button_clicked() {
	process(CONTINUE);
}

void on_num_dialog_submit_button_clicked() {
	char* begin = dialog_buffer;
	char* end = begin;
	if (!strlen(begin)) { return; }

	while(*end > '0' - 1 && *end < '9' + 1) end++;
	if (end == begin) return;
	long int return_val = strtol(begin, &end, 10);
	if (val_min <= return_val && return_val <= val_max) {
		dialog_value = return_val;
		process(CONTINUE);
	}
}

void on_num_dialog_cancel_button_clicked() {
	process(CANCEL);
}

void on_num_dialog_input_field_changed(GtkEntry* e) {
	strncpy(dialog_buffer, gtk_entry_get_text(e), 50);
}

void on_window_main_destroy() {
	gtk_main_quit();
}

void on_button_main_clicked() {
	process(START);
}
