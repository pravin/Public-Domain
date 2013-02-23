/* Node.js Application */
var http = require('http');
var parse = require('url').parse;

var SERVER = '127.0.0.1';
var PORT = 8080;

var id = 0; /* You can start from a non-zero seed */
var url_to_index = new Array();
var short_to_url = new Array();

/* Randomize CHARS if you dont want people to guess the next url generated */
var CHARS = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHUJKLMNOPQRSTUVWXYZ';

function num_to_base62(n) {
    if(n > 62) {
        return num_to_base62(Math.floor(n / 62)) + CHARS[n % 62];
    } else {
        return CHARS[n];
    }
}

var srv = http.createServer(function(req, res) {
    var param = parse(req.url, true);

    if (param.pathname == '/add') {
        if (param.query.url != undefined) { /* We have a url to add */
            /* Check whether the url has been added before */
            short_url = url_to_index[param.query.url];
            if (short_url == undefined) { /* Nope, not added */
                short_url = num_to_base62(id);
                while (short_url.length < 5) { /* Add padding */
                    short_url = CHARS[0] + short_url;
                }
                url_to_index[param.query.url] = short_url;
                short_to_url[short_url] = param.query.url;
                id++;
            }
            res.writeHead(200, {'Content-Type': 'text/html'});
            /* You may want to drop PORT in production environments */
            short_url_string = 'http://' + SERVER + ':' + PORT + 
                               '/' + short_url;
            res.end('Your short url is: <a href="' + short_url_string +
                '">' + short_url_string + '</a>');
        }
    } else { /* Redirect user to the right place */
        long_url = short_to_url[param.pathname.substring(1)];
        if (long_url != undefined) {
            res.writeHead(302, {'Location': long_url});
            res.end();
        } else {
            res.writeHead(404, {'Content-Type': 'text/plain'});
            res.end('404 - Requested url not found');
        }
    }
}).listen(PORT, SERVER);

console.log('Server running at http://' + SERVER + ':' + PORT + '/');