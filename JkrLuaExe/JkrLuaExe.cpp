﻿#define SOL_PRINT_ERRORS 1
#define SOL_ALL_SAFETIES_ON 1
#include "JkrLuaExe.hpp"

namespace JkrEXE {
extern void CreateMainBindings(sol::state& inState);
extern void CreateGLMBindings(sol::state& inState);
extern void CreateKeyBindings(sol::state& inState);
extern void CreateMiscBindings(sol::state& inState);
extern void CreateRendererBindings(sol::state& inState);

void CreateMainBindings(sol::state& s) {
	     CreateGLMBindings(s);
	     CreateKeyBindings(s);
	     CreateMainBindings(s);
	     CreateRendererBindings(s);
	     CreateMiscBindings(s);
}
} // namespace JkrEXE

int main(int ArgCount, char** ArgStrings) {
	     using namespace JkrEXE;
	     sol::state s;
	     s.open_libraries();
	     CreateMainBindings(s);

	     const vector<string_view> CommandLineArgs(ArgStrings + 1, ArgStrings + ArgCount);
	     s.set_exception_handler(&my_exception_handler);
	     sol::protected_function_result result = s.safe_script_file("app.lua", &sol::script_pass_on_error);
	     if (not result.valid()) {
			sol::error error = result;
			std::cout << error.what();
			exit(EXIT_FAILURE);
	     }
	     return 0;
}
