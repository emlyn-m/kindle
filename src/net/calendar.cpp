#include "../widgets/widgets.hpp"
#include "../secrets.h"

#include "net.hpp"
#include "cJSON.h"
#include <cmath>
#include <cstdlib>
#include <cstring>

#define JWT_REDEEM_URL "https://oauth2.googleapis.com/token"


void update_events(gpointer* calendar_gp) {
        
    const uint32_t TOKEN_BUFSIZE = 2048;
    calendar_t* cal = (calendar_t*) calendar_gp;
    long ctime;
    if ((ctime = time(NULL)) > cal->token_exp) {
        printf("token expired, renewing\n");        
        
        if (!cal->token_buf) {
            cal->token_buf = (char*) malloc(TOKEN_BUFSIZE * sizeof(char)); 
            memset(cal->token_buf, 0, TOKEN_BUFSIZE);
        }
        
        int token_result = generate_gcal_jwt((char*) SERVICE_EMAIL, (char*) PRIVKEY, TOKEN_BUFSIZE, cal->token_buf);
        if (token_result) { 
            fprintf(stderr, "failed to generate jwt\n");
            return; 
        } // error!!
        
        const uint32_t token_redeem_bufsize = 2048;
        char* token_redeem_payload = (char*) malloc(token_redeem_bufsize * sizeof(char));
        snprintf(token_redeem_payload, token_redeem_bufsize, "curl -sX POST -H \"Content-Type: application/x-www-form-urlencoded\" -d \"grant_type=urn%%3Aietf%%3Aparams%%3Aoauth%%3Agrant-type%%3Ajwt-bearer&assertion=%s\" %s", cal->token_buf, JWT_REDEEM_URL);
        FILE* token_redeem_fp = popen(token_redeem_payload, "r");
        if (!token_redeem_fp) { /* failed to exec -  yikes! */ 
            fprintf(stderr, "failed to exec token redeem");
            return; 
        }
        
        const uint32_t token_resp_bufsize = 2048;
        char* token_resp_buf = (char*) malloc(token_resp_bufsize * sizeof(char));
        fgets(token_resp_buf, token_resp_bufsize, token_redeem_fp);
        
        
        cJSON* token_resp_j = cJSON_Parse(token_resp_buf);

        int token_expiry = floor(cJSON_GetObjectItem(token_resp_j, "expires_in")->valuedouble);
        char* token_value = cJSON_GetObjectItem(token_resp_j, "access_token")->valuestring;
        
        printf("token value: %s (expiring in %d seconds)\n", token_value, token_expiry);
        
        cal->token_exp = ctime + token_expiry;
        
        
        
        
    }
}
