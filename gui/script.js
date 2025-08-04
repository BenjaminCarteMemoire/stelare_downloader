window.addEventListener('DOMContentLoaded', () => {

    let webui_connected = false;
    let drive_letters = null;

    function wait_webui(retry, callback) {

        if (typeof webui === 'undefined') {
            console.error('WebUi not defined.');
            return;
        }

        if (webui.isConnected()) {
            console.log("Webui connected");
            webui_connected = true;
            if (typeof callback !== 'undefined')
                callback();
            return;
        } else if (retry > 0) {
            console.log("Waiting for login...")
            setTimeout(() => wait_webui(retry - 1, callback), 500);
        } else {
            console.log("Too much tries, abort.");
            return;
        }

    }

    function when_webui_is_connected(){

        if( !webui_connected ) return;
        console.log( "OK!" );

        console.log( window.location.href );
        console.log( window.location.href.includes( "select_drive.html" ) );

        if( window.location.href.includes( "select_drive.html" ) ) {

            // Get drive letters available.
            webui.call('spc_drive_letters')
                .then(response => {
                    console.log( response );
                    drive_letters = JSON.parse(response);
                    console.log(drive_letters);
                    drive_letters_as_select();
                });

        }

    }

    function drive_letters_as_select(){

        const selector = document.getElementById("drive_selector");

        drive_letters.forEach( (letter) => {
            const opt = document.createElement("option");
            opt.text = letter; opt.value = letter;
            selector.add(opt, null);
        });

        document.getElementById( "valid_drive_letter_selection" ).onclick = function( e ){

           if( typeof selector.value == "undefined" || selector.value == "" )
               return;

           webui.call("continue_package", selector.value);
           // window.close();

        }

    }

    wait_webui( 10, when_webui_is_connected );

});