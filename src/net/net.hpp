#include "../widgets/widgets.hpp"

int http_get(char* hostname, char* path, int port, char** out);
int http_post(char* hostname, char* path, int port, char* content_type, char* body, char** out);

void update_events(gpointer*);
int generate_gcal_jwt(char* service_email, char* privkey, int out_size, char* out);
