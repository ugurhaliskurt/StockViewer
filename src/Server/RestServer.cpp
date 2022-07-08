#include "Server/RestServer.hpp"
#include <json.hpp>


handler::handler()
{
    //ctor
}
handler::handler(utility::string_t url):m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&handler::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&handler::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&handler::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&handler::handle_delete, this, std::placeholders::_1));
    m_listener.support(methods::OPTIONS, std::bind(&handler::handle_options, this, std::placeholders::_1));

}
handler::~handler()
{
    //dtor
}

void handler::handle_error(pplx::task<void>& t)
{
    try
    {
        t.get();
    }
    catch(...)
    {
        // Ignore the error, Log it if a logger is available
    }
}


//
// Get Request 
//
void handler::handle_get(http_request message)
{
    ucout <<  message.to_string() << endl;

    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    message.relative_uri().path();
	//Dbms* d  = new Dbms();
    //d->connect();

    concurrency::streams::fstream::open_istream(U("static/index.html"), std::ios::in).then([=](concurrency::streams::istream is)
    {
        message.reply(status_codes::OK, is,  U("text/html"))
		.then([](pplx::task<void> t)
		{
			try{
				t.get();
			}
			catch(...){
				//
			}
	});
    }).then([=](pplx::task<void>t)
	{
		try{
			t.get();
		}
		catch(...){
			message.reply(status_codes::InternalError,U("INTERNAL ERROR "));
		}
	});

    return;

};

//
// A POST request
//
void handler::handle_post(http_request message)
{
    ucout <<  message.to_string() << endl;

    nlohmann::json parsed_data = nlohmann::json::parse( message.extract_string().get() );
    for( auto &element : parsed_data )
    {
        auto val = element["Name"].get<std::string>();
        std:: cout << val << std::endl;
    }

    message.reply(status_codes::OK,message.to_string());
    return ;
};

//
// A DELETE request
//
void handler::handle_delete(http_request message)
{
     ucout <<  message.to_string() << endl;

        string rep = U("WRITE YOUR OWN DELETE OPERATION");
      message.reply(status_codes::OK,rep);
    return;
};


//
// A PUT request 
//
void handler::handle_put(http_request message)
{
    ucout <<  message.to_string() << endl;
     string rep = U("WRITE YOUR OWN PUT OPERATION");
     message.reply(status_codes::OK,rep);
    return;
};

void handler::handle_options(http_request request)
{
    http_response response(status_codes::OK);
    response.headers().add(U("Allow"), U("GET, POST, PUT, DELETE, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    response.headers().add(U("Access-Control-Allow-Methods"), U("GET, POST, PUT, DELETE, OPTIONS"));
    response.headers().add(U("Access-Control-Allow-Headers"), U("Content-Type"));
    request.reply(response);
}

void handler::on_initialize(const string& address)
{
    uri_builder uri(address);
  

    auto addr = uri.to_uri().to_string();
    open().wait();

    ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

    return;
}

void handler::on_shutdown()
{
    close().wait();
    return;
}