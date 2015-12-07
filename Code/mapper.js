// Copyright 2015 StrainSource LLC
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.


// This code sample is from my previous project StrainSource LLC, a dispensary finder and cannabis strain information resource.
// This is a section of the code from the dispensary map page.  I have an implemented, slightly older version of this code up on an AWS server at http://52.25.225.79/Dispensaries

var dispensaryMap = {
    // Start by initializing variables for the map page to use
    // allMarkers is an array to be filled with all the possible markers on the map.
    allMarkers: {},

    // set up user control buttons and text fields, and their functions,
    // then load the mapping data.
    init: function() {
        // InfoWindow is the information popup that appears on the map when something is clicked.
        dispensaryMap.InfoWindow = new google.maps.InfoWindow( {
            content: ''
        } );

        $( '#dispensary-license-search' ).buttonset();
        $( ".mmjc-searchbox" ).find( "input:text" ).textReplacement();
        $( ".mmjc-searchbox li" ).show();
        //Set up card click actions
        //only follow  'a' links if card is selected already, first click selects, second follows
        $( '.flex-container.forMap' ).on( 'click', '.mmjc-card a', function( event ) {
            if ( !$( this ).parents( '.mmjc-card.strain-card' ).first().hasClass( 'selected' ) ) {
                event.preventDefault();
            }
        } );
        //first click on card selects that marker on map and loads infoWindow.
        $( '.flex-container.forMap' ).on( 'click', '.flex-item', function() {
            var id = $( this ).attr( 'id' );
            dispensaryMap.loadInfo( id );
        } );
        //continue loading map page
        dispensaryMap.initMap();
        dispensaryMap.loadResults();
    },

    // Query the webserver for a list of all the dispensaries, using JQuery shortcuts for AJAX
    // and return them in JSON format.
    loadResults: function() {
        // get ALL the dispensaries
        $.getJSON(
            '/dispensaries/ajaxIndexAllDispensaries',
            function( json ) {
                //take the returned JSON data and pass it to the flex container to populate the info cards.
                $( '.flex-container.forMap' ).flexContainer( {
                    items: json.result,
                    itemTemplate: '#mmjcCard'
                } );
            } ).done( function() {
            //hide the page loading placeholders
            $( ".strain-div.no-search-results" ).hide();
            console.info( "load map results: Done" );
        } );
    },

    //Set up the Google map via the API from Google.
    initMap: function() {
        var initialLocation = new google.maps.LatLng( 39.7391536, -104.9847034 ); //Denver
        var myOptions = {
            zoom: 9, //front range zoom level
            mapTypeId: google.maps.MapTypeId.ROADMAP, //map shows roads
            center: initialLocation
        };
        //draw map
        tallMap = new google.maps.Map( document.getElementById( "map_canvas" ), myOptions );
        console.info( "map initialized." );
    },

    // this gets called by the flexcontainer.js file, after it has received the JSON dispensary data and drawn the cards into the flex box
    makeMarkers: function( items ) {
        //TODO this needs to be ROBUSTIFIED for no address/no lat,lng data in the DB
        if ( typeof items !== 'undefined' && items !== null ) {
            //set up parameters for various map sprites
            var spriteAnchor = new google.maps.Point( 10, 8 );
            var smallSprite = new google.maps.Size( 20, 20 );
            var bigSprite = new google.maps.Size( 80, 80 );
            var spritePos = {
                rec: new google.maps.Point( 1, 4 ),
                both: new google.maps.Point( 3, 38 ),
                med: new google.maps.Point( 3, 72 ),
                wholesale: new google.maps.Point( 3, 107 ),
                wut: new google.maps.Point( 10, 148 )
            };
            var spriteSize = {
                rec: new google.maps.Size( 37, 35 ),
                both: new google.maps.Size( 33, 33 ),
                med: new google.maps.Size( 33, 33 ),
                wholesale: new google.maps.Size( 33, 33 ),
                wut: new google.maps.Size( 20, 20 )
            };
            //SS is a global variable set up by a grunt task on deployment, with cachebusted urls of all the images used on the site
            var spriteUrl = SS.mapIconSprite;

            for ( var i = 0; i < items.length; i++ ) {

                var id = items[ i ].Dispensary.id;
                console.info( "makemarkers ID:" + id + " -> " + items[ i ].Dispensary.name );
                // address is currently unused as addresses are converted to lat/longs in a cron job
                // var address = $('#dispensary-address-' + id).val();
                var lat = items[ i ].Dispensary.lat;
                var lng = items[ i ].Dispensary.lng;
                var license = items[ i ].Dispensary.license;
                var status = items[ i ].Dispensary.status;
                var latlng = new google.maps.LatLng( lat, lng );
                //Setup Marker Image
                var markerImage = {};
                //zeeIndex is for sorting the stacking order of sprites on the map
                var zeeIndex;

                if ( status == "ACTIVE" || status == "PRELOADED" ) {
                    zeeIndex = parseInt( id ) + 1000;
                    //this switch is for hooking different business licesnse types to their sprites
                    switch ( license ) {
                        case "1":
                            //medical only dispensaries
                            markerImage = {
                                url: spriteUrl,
                                size: spriteSize.med,
                                origin: spritePos.med,
                                anchor: spriteAnchor,
                                scaledSize: bigSprite
                            };
                            break;
                        case "2":
                            //recreational only
                            markerImage = {
                                url: spriteUrl,
                                size: spriteSize.rec,
                                origin: spritePos.rec,
                                anchor: spriteAnchor,
                                scaledSize: bigSprite
                            };
                            break;
                        case "3":
                            //both med and rec
                            markerImage = {
                                url: spriteUrl,
                                size: spriteSize.both,
                                origin: spritePos.both,
                                anchor: spriteAnchor,
                                scaledSize: bigSprite
                            };
                            break;
                        case "4":
                            //dispensaries open to wholesale sales (b2b)
                            markerImage = {
                                url: spriteUrl,
                                size: spriteSize.wholesale,
                                origin: spritePos.wholesale,
                                anchor: spriteAnchor,
                                scaledSize: bigSprite
                            };
                            break;
                        case "5":
                            //All (med, rec, wholesale)
                            markerImage = {
                                url: spriteUrl,
                                size: spriteSize.both,
                                origin: spritePos.both,
                                anchor: spriteAnchor,
                                scaledSize: bigSprite
                            };
                            break;
                        case "null":
                            //fall through if license is unset in the DB
                            markerImage = {
                                url: spriteUrl,
                                size: spriteSize.wut,
                                origin: spritePos.wut,
                                anchor: spriteAnchor,
                                scaledSize: bigSprite
                            };
                            break;
                        default:
                            // hopefully nothing ends up here, but just in case
                            markerImage = {
                                url: spriteUrl,
                                size: spriteSize.wut,
                                origin: spritePos.wut,
                                anchor: spriteAnchor,
                                scaledSize: smallSprite
                            };
                            break;
                    }
                }

                if ( status == "PRELOADED" ) {
                    //preloaded circle for dispensaries without menus
                    markerImage.scaledSize = smallSprite;
                    zeeIndex = parseInt( id ) + 100;
                }

                if ( status !== "ACTIVE" && status !== "PRELOADED" ) {
                    //Unapprpved, deactivated or otherwise strange dispensaries
                    zeeIndex = parseInt( id ) + 100;
                    markerImage = {
                        url: spriteUrl,
                        size: spriteSize.wut,
                        origin: spritePos.wut,
                        anchor: spriteAnchor,
                        scaledSize: smallSprite
                    };
                }
                // set up marker for current dispensary
                var marker = new google.maps.Marker( {
                    visible: false,
                    map: tallMap,
                    position: latlng,
                    icon: markerImage,
                    zIndex: zeeIndex,
                    title: items[ i ].Dispensary.name
                } );

                dispensaryMap.allMarkers[ id ] = marker;
            }
            //use Google API to attach click event listeners to each marker
            $.each( dispensaryMap.allMarkers, function( index ) {
                google.maps.event.addListener( this, 'click', function() {
                    console.info( "clicked marker: " + index );
                    dispensaryMap.loadInfo( index );
                } );
            } );
        }
    },

    // this gets run when either the flex card or the map marker is clicked
    loadInfo: function( id ) {
        var InfoWindow = dispensaryMap.InfoWindow;

        //attach event listener so that clicking anywhere else on map closes infowindow
        google.maps.event.addListener( tallMap, 'click', function() {
            InfoWindow.close();
        } );

        // Style the flex box cards responding to click event
        $( '.mmjc-card' ).removeClass( 'selected' );
        $( '.flex-item#' + id + ' .mmjc-card' ).addClass( 'selected' );

        //if the marker is clicked, scroll the flex box to show the corresponding card
        var itemOffset = $( '.flex-content .flex-item#' + id ).offset().top;
        var containerTop = $( '.flex-content' ).css( 'top' );
        console.log( "itemOffset " + itemOffset + "containerTop: " + containerTop );
        var scrollstop = ( itemOffset * -1 ) - containerTop;

        $( '.flex-content' ).animate( {
            'top': scrollstop
        }, 1000 );

        // pull particulare dispensaries marker
        var markThis;
        markThis = dispensaryMap.allMarkers[ id ];
        // zoom and center map on marker
        tallMap.setZoom( 14 );
        tallMap.setCenter( markThis.position );
        // build html to insert into the infoWindow
        var address = $( '#dispensary-address-' + id ).val();
        var name = $( '#dispensary-name-' + id ).val();
        var contentString = '<div id="wrapper-map-infowindow-content-' + id + '" class="dispensary-row"/>' +
            '<div class="map-infowindow-content">' +
            '<div class="infowindow-image outerContainer left-column">' +
            '<div class="innerContainer">' +
            '<img src="/Dispensaries/getDispensaryLogo/' + id + ' "/>' +
            '</div>' +
            '</div>' +
            '<div class="infowindow-name outerContainer">' +
            '<div class="innerContainer">' +
            '<a href="/Dispensaries/view/' + id + '" > ' + name + '</a>' +
            '</div>' +
            '</div>' +
            '<div class="infowindow-address outerContainer" >' +
            '<div class="innerContainer">' +
            '<span>' +
            address +
            '</span>' +
            '</div>' +
            '</div>' +
            '<div class="clear"></div>' +
            '<a href="/Dispensaries/view/' + id + '" class="infowindow-overlink"><span></span></a>' +
            '</div>' +
            '</div>';
        // if a infoWindow is open, close it, reset data and open new one, else just open the first one
        if ( InfoWindow ) {
            InfoWindow.close();
            InfoWindow.setContent( contentString );
            InfoWindow.open( tallMap, markThis );
        } else {
            InfoWindow = new google.maps.InfoWindow( {
                content: contentString
            } );
        }
    }
};
