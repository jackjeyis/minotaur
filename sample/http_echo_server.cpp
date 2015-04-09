/**
 * @file http_echo_server.cpp
 * @author Wolfhead
 */
#include <io_service.h>
#include <service/service.h>
#include <service/coroutine_service_handler.h>
#include <coroutine/coro_all.h>
#include <net/io_descriptor_factory.h>
#include <net/acceptor.h>
#include <application/generic_application.h>
#include <application/config_manager.h>
#include <client/client_manager.h>

using namespace minotaur;
LOGGER_STATIC_DECL_IMPL(logger, "root");

int main(int argc, char* argv[]) {
  GenericApplication<ConfigManager, CoroutineServiceHandler> app;
  Client client(app.GetIOService(), "rapid://localhost:6602", 1000);

  return 
    app
      .SetOnStart([&](){
        client.Start();

        app.RegisterService("http_echo_handler", [&](ProtocolMessage* message) {
          //coro::StartTimer(10000);
          //coro::Yield();
          //ClientRouter* router = app.GetClientManager()->GetClientRouter("rapid");
          RapidMessage* rapid_message = MessageFactory::Allocate<RapidMessage>();
          rapid_message->body = "test";

          RapidMessage* response = client.SendRecieve(rapid_message);
          if (response) {
            LOG_INFO(logger, "rapid response:" << response->body);
          }
          coro::Send(message);
        });
        return 0;
      })
      .Run(argc, argv);
}
