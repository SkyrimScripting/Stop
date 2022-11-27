#include <spdlog/sinks/basic_file_sink.h>

void InitializeLog() {
	auto path = SKSE::log::log_directory();
	if (!path) SKSE::stl::report_and_fail("Failed to find standard logging directory"sv);
	*path /= fmt::format("{}.log", SKSE::PluginDeclaration::GetSingleton()->GetName());
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
	const auto level = spdlog::level::trace;
	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(level);
	log->flush_on(level);
	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
	InitializeLog();
	SKSE::log::info("Hold onto your butts!");

	SKSE::Init(skse);

	// Once all plugins and mods are loaded, then the ~ console is ready and can be printed to
	SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message){
		if (message->type == SKSE::MessagingInterface::kDataLoaded) {
			SKSE::log::info("kDataLoaded, closing Skyrim!");
			SKSE::WinAPI::TerminateProcess(SKSE::WinAPI::GetCurrentProcess(), EXIT_SUCCESS);
		}
	});

	return true;
}