
Pebble.addEventListener( 'ready', function() {
    console.log( 'JSReady' );
    var dict = { 'JSReady': true };
    Pebble.sendAppMessage( dict, function() {}, function( e ) {} );
} );

Pebble.addEventListener( 'appmessage', function( e ) {
    console.log( 'Received message: ' + e );
    
    var dict = e.payload;
    
    if ( "RequestNewImage" in dict )
    {
        sendNewImageRequest();
    }
} );

function sendNewImageRequest() {
    console.log( 'Requesting new image' );
    var request = new XMLHttpRequest();
    request.onLoad = function() { getImageData( this.responseText ) };
    request.open( 'GET', 'https://inspirobot.me/api?generate=true' );
    request.send();
}

function getImageData( url ) {
    console.log( 'Getting image data: ' + url );
    
    var request = new XMLHttpRequest();
    request.onLoad = function() { console.log( this.response ) };
    request.open( 'GET', url );
    request.send();
}
