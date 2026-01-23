#include "../widgets/widgets.hpp"
#include "net.hpp"
#include <cstdlib>
#include <cstring>

#define CALENDAR_ID "emlynmatheson@gmail.com"
#define SERVICE_EMAIL "cal-update@clean-emblem-485106-h0.iam.gserviceaccount.com"
#define PRIVKEY "-----BEGIN PRIVATE KEY-----\nREDACTED-----END PRIVATE KEY-----\n"
#define JWT_REDEEM_URL "https://oauth2.googleapis.com/token"


void update_events(gpointer* calendar_gp) {
    
    // test
    // http_post((char*) "httpbin.org", (char*) "post", 80, (char*) "application/x-www-form-urlencoded", (char*) "p1=a&p2=b", &body);
    
    const uint32_t TOKEN_BUFSIZE = 2048;
    calendar_t* cal = (calendar_t*) calendar_gp;
    if (time(NULL) > cal->token_exp) {
        if (!cal->token_buf) {
            cal->token_buf = (char*) malloc(TOKEN_BUFSIZE * sizeof(char)); 
            memset(cal->token_buf, 0, TOKEN_BUFSIZE);
        }
        
        int token_result = generate_gcal_jwt((char*) SERVICE_EMAIL, (char*) PRIVKEY, TOKEN_BUFSIZE, cal->token_buf);
        if (!token_result) { return; } // error!!
        
        const uint32_t token_redeem_bufsize = 2048;
        char* token_redeem_payload = (char*) malloc(token_redeem_bufsize * sizeof(char));
        snprintf(token_redeem_payload, token_redeem_bufsize, "grant_type=urn%%3Aietf%%3Aparams%%3Aoauth%%3Agrant-type%%3Ajwt-bearer&assertion=%s", cal->token_buf);
        char* token = NULL;
        // http_post((char*) "oauth2.googleaps.com", (char*) "token", 443, (char*) "application/x-www-form-urlencoded", token_redeem_payload, &token); //todo: fuck this doesnt handle ssl...
    }
}
