// https://www.scaledrone.com/blog/webrtc-tutorial-simple-video-chat/

// Generate random room name if needed
if (!location.hash) {
    location.hash = Math.floor(Math.random() * 0xFFFFFF).toString(16);
}
const roomHash = location.hash.substring(1);

// Configuration is passed to RTCPeerConnection instance. Use Google's STUN server
const configuration = {
    iceServers: [{
        urls: 'stun:stun.l.google.com:19302' // Google's public STUN server
    }]
};

// Callbacks for error handling
function onSuccess() {};
function onError(error) {
    console.error(error);
};

// Scaledrone works by letting you subscribe to a room, then broadcast messages sent
// to all subscribed users.
// To use, add this before </head> tag: 
//      <script type='text/javascript' src='https://cdn.scaledrone.com/scaledrone.min.js'></script>

// Subscribe to a room
// Room name needs to be prefixed with 'observable-'
const roomName = 'observable-' + roomHash;
let room;
const drone = new ScaleDrone('yiS12Ts5RdNhebyM');

drone.on('open', error => {
    if (error) {
        return onError(error);
    }
    room = drone.subscribe(roomName);
    room.on('open', error => {
        if (error) {
            onError(error);
        }
    });
    // Connected to room and received array of members
    room.on('members', members => {
        if (members.length >= 3) {
            return alert('The room is full');
        }
        // If we are second user to connect
        const isOfferer = members.length === 2;
        startWebRTC(isOfferer);
        //startListeningToSignals();
    });
});

// Send signaling data via Scaledrone
function sendMessage(message) {
    drone.publish({
        room: roomName,
        message
    });
}

/**
 Video and Audio streaming code
 */

let pc;
function startWebRTC(isOfferer) {
    pc = new RTCPeerConnection(configuration);
    
    // 'onicecandidate' notifies us whenever an ICE agent needs to deliver a
    // message to the other peer through the signaling server
    pc.onicecandidate = event => {
        if (event.candidate) {
            sendMessage({'candidate': event.candidate});
        }
    };
    
    // If user is offerer, let the 'negotiationneeded' event create the offer
    if (isOfferer) {
        pc.onnegotiationneeded = () => {
            pc.createOffer().then(localDescCreated).catch(onError);
        }
    }
    
    /*
    // When a remote stream arrives display it in the #remoteVideo element
    pc.onaddstream = event => {
        remoteVideo.srcObject = event.stream;
    };
    */
    
    // NOT IN TUTORIAL
    // When a remote stream arrives display it in the #remoteVideo element
    pc.ontrack = event => {
        const stream = event.streams[0];
        if (!remoteVideo.srcObject || remoteVideo.srcObject.id !== stream.id) {
            remoteVideo.srcObject = stream;
        }
    };
    
    navigator.mediaDevices.getUserMedia({
        audio: true,
        video: true,
    }).then(stream => {
        
        // Display your local video in #localVideo element
        localVideo.srcObject = stream;
        
        // Add your stream to be sent to the connecting peer
        //pc.addStream(stream);
        stream.getTracks().forEach(track => pc.addTrack(track, stream));
    }, onError);
    
    // NOT IN TUTORIAL
    // Listen to signaling data from Scaledrone
    room.on('data', (message, client) => {
        // Message was sent by me
        if (client.id === drone.clientId) {
            return;
        }
        if (message.sdp) {
            // Called after receiving an offer or answer from another peer
            pc.setRemoteDescription(new RTCSessionDescription(message.sdp), () => {
                // When receiving an offer, answer it
                if (pc.remoteDescription.type === 'offer') {
                    pc.createAnswer().then(localDescCreated).catch(onError);
                }
            }, onError);
        }
        else if (message.candidate) {
            // Add new ICE candidate to our connections remote description
            pc.addIceCandidate(
                new RTCIceCandidate(message.candidate), onSuccess, onError
            );
        }
    });
}

// Define the startListeningToSignals function to listen to messages from signaling service
// We're listening for two kinds:
//      message.sdp - Sessoin Description Protocol, describes local end of remote connection
//      message.candidate - Add new ICE candidate to connections
function startListeningToSignals() {
    // Listen to signaling data from Scaledrone
    room.on('data', (message, client) => {
        
        // Message was sent by us
        if (!client || client.id === drone.clientId) {
            return;
        }
        if (message.sdp) {
            // This is called after receiving an offer or answer from another peer
            pc.setRemoteDescription(new RTCSessionDescription(message.sdp), () => {
                // When receiving an offer lets answer it
                if (pc.remoteDescription.type === 'offer') {
                    pc.createAnswer().then(localDescCreated).catch(onError);
                }
            }, onError);
        }
        else if (message.candidate) {
            //Add new ICE candidate to our connections remote description
            pc.addIceCandidate(
                new RTCIceCandidate(message.candidate), onSuccess, onError
            );
        }
    });
}

// this gets called when creating an offer and when answering one. It updates local description of connection
function localDescCreated(desc) {
    pc.setLocalDescription(
        desc,
        () => sendMessage({'sdp': pc.localDescription}),
        onError
    );
}