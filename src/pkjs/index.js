
Pebble.addEventListener( 'ready', function() {
    setTimeout( () => {
        console.log( 'JSReady' );
        var dict = { 'JSReady': true };
        Pebble.sendAppMessage( dict, function( e ) {}, function( e ) {} );
    }, 3000 );
} );

Pebble.addEventListener( 'appmessage', function( e ) {
    var dict = e.payload;
    console.log( 'Received message: ' + dict );
    
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
