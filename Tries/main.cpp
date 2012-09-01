#include <stdio.h>
#include <stdlib.h>
#include <microhttpd.h>
#include <string.h>
#include <btree.h>

#define PORT    8095
#define GET     0
#define POST    1

BTree *btree = NULL; // Stores the root of the BTree
char output[512];    // Stores the output of

static int post_processor(char *upload_data, size_t upload_data_size) {
    return MHD_YES;
}


int answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **con_cls)
{
    const char *getPage = "GET method not supported";
    struct MHD_Response *response;
    int ret;

    printf ("New %s request for %s using version %s\n", method, url, version);

    if (strcmp(method, "GET") == 0) { // Return not-supported
        response = MHD_create_response_from_data(strlen(getPage), (void*) getPage, MHD_NO, MHD_NO);
        ret = MHD_queue_response(connection, MHD_HTTP_FORBIDDEN, response);
        MHD_destroy_response(response);
    }
    /*
    else { // Post method
        if (*upload_data_size != 0) {
            MHD_post_process(post_processor, upload_data, *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        }
    }
    */

    return ret;
}


bool parseOpts(int argc, char *argv[]) {
    bool retval = false;
    if (argc > 1) {
        retval = true;
        char *c = argv[1]; // Use the first command line parameter
        while(*c != 0) {
            if (*c > 57 || *c < 48) {
                retval = false;
                break;
            }
            c++;
        }
    }
    return retval;
}

void usage() {
    printf("Usage: BTree <port>\n");
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen(argv[1], "r");

    btree = new BTree();

    btree->addPhrase("Hello");
    btree->match("Hello");

    delete(btree);

    return 0;
}
