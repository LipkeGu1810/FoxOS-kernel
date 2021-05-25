#include <shell/shell.h>
#include <renderer/font_renderer.h>
#include <string.h>
#include <paging/page_frame_allocator.h>

using namespace shell;

Shell* shell::global_shell;

Shell::Shell() {
	
}

void Shell::init_shell() {
	command_buffer = (char*)global_allocator.request_page();
	buffer_len = 0;
	print_nl();
}

void Shell::print_nl() {
	renderer::global_font_renderer->printf("\nFoxOS > ");
}

void Shell::run_command() {
	if (buffer_len == 0) {
		print_nl();
		return;
	}

	if (strcmp(command_buffer, "test") == 0) {
		renderer::global_font_renderer->printf("\nHello World!");
	} else {
		renderer::global_font_renderer->printf("\nUnknown command: %s", command_buffer);
	}

	memset(command_buffer, 0, 4096);
	buffer_len = 0;
	print_nl();
}

void Shell::keypress(char key) {
	if (key == 0) {
		return;
	}

	if (key == '\n') {
		run_command();
	} else if (key == '\b') {
		if (buffer_len > 0) {
			renderer::global_font_renderer->printf("\b");
			buffer_len--;
			command_buffer[buffer_len] = 0;
		}
	} else {
		if (buffer_len < 4096) {
			renderer::global_font_renderer->printf("%c", key);
			command_buffer[buffer_len] = key;
			buffer_len++;
		}
	}
}