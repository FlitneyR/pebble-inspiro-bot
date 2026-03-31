const MAX_MESSAGE_SIZE = 8000;

Pebble.addEventListener( 'ready', function() {
    console.log( 'JSReady' );
    var dict = { 'JS_READY': true };
    Pebble.sendAppMessage( dict, function( e ) {}, function( e ) {} );
} );

Pebble.addEventListener( 'appmessage', function( e ) {
    var dict = e.payload;
    console.log( 'Received message: ' + dict );
    
    if ( dict[ 'REQUEST_NEW_IMAGE' ] )
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

function sendImage( width, height, data ) {
    var dict = {
        'IMAGE_WIDTH': width,
        'IMAGE_HEIGHT': height,
        'IMAGE_DATA_SIZE': data.length
    };
    
    Pebble.sendAppMessage( dict, function( e ) {}, function( e ) {} );
    
    for ( var offset = 0; offset < MAX_MESSAGE_SIZE; offset += MAX_MESSAGE_SIZE )
    {
        var dict = { 'IMAGE_DATA_PART': data.slice( offset, MAX_MESSAGE_SIZE ) };
        Pebble.sendAppMessage( dict, function( e ) {}, function( e ) {});
    }
}
