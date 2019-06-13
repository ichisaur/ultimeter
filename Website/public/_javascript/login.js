var people = [
    {
        email: "student0@u.northwestern.edu",
        password: "student0",
        name: "John Doe"
    },
    {
        email: "student1@u.northwestern.edu",
        password: "student1",
        name: "Joe Smith"
    }
]

var config = {
    authDomain: "northwestern-task.firebaseapp.com",
    databaseURL: "https://northwestern-task.firebaseio.com"
}
if (!firebase.apps.length) {
    firebase.initializeApp(config);
}
var database=firebase.database();

function formSubmit() {
    if (document.getElementById("emailval").value != "" &&
        document.getElementById("passwordval").value != "") {

        var email = document.getElementById("emailval").value;
        var password = document.getElementById("passwordval").value;

        email = encodeURIComponent(email).replace(/\./g, '%2E');

        database.ref('/users/' + email).once('value').then(function(snapshot){

            checkLogin(email, password, snapshot);
        })
        // to decode: 
        //replace('%2E', '.') and a simple decodeURIComponent(...)

        // for (let i = 0; i < people.length; i++) {
        //     if (email == people[i].email && password == people[i].password) {
        //         document.location.href = 'task' + i + '.html';

        //         window.localStorage.setItem("userID", i);
        //         return;
        //     }
        // }
        // If wrong login

    }    
    else {
        document.getElementById("emailval").setAttribute("class","input is-danger");
        document.getElementById("passwordval").setAttribute("class","input is-danger");
        document.getElementById("badlogin").style.display = "inline";
    }
}

function checkLogin(_email, _password, _snapshot) {
    if (_snapshot.val() != null) {
        if (_password == _snapshot.val().password) {
            document.cookie = "username=" + _email;
            document.cookie = "name=" + _snapshot.val().name;
            document.location.href='tasklist.html';
        }
        else {
            document.getElementById("emailval").setAttribute("class","input is-danger");
            document.getElementById("passwordval").setAttribute("class","input is-danger");
            document.getElementById("badlogin").style.display = "inline";
        }
    }
    else {
        document.getElementById("emailval").setAttribute("class","input is-danger");
        document.getElementById("passwordval").setAttribute("class","input is-danger");
        document.getElementById("badlogin").style.display = "inline";
    }
}


document.addEventListener('DOMContentLoaded', function () {
    var user_name = document.cookie.replace(/(?:(?:^|.*;\s*)name\s*\=\s*([^;]*).*$)|^.*$/, "$1")
    var user_id = document.cookie.replace(/(?:(?:^|.*;\s*)username\s*\=\s*([^;]*).*$)|^.*$/, "$1")
    if (user_name != "" && user_id != "") {
        document.location.href='tasklist.html';
    }
})


function registerComplete() {

    document.getElementById('register-email').setAttribute("class", "input");
    document.getElementById('register-first').setAttribute("class", "input");
    document.getElementById('register-last').setAttribute("class", "input");
    document.getElementById('register-password').setAttribute("class", "input");
    document.getElementById('register-confirm-password').setAttribute("class", "input");
    document.getElementById("badlogin").style.display = "none";
    document.getElementById("badterms").style.display = "none";
    document.getElementById("badpass").style.display = "none";
    document.getElementById("bademail").style.display = "none";

    var email = document.getElementById('register-email').value;
    var emailEncoded = encodeURIComponent(email).replace(/\./g, '%2E');

    var firstName = document.getElementById('register-first').value;
    var lastName = document.getElementById('register-last').value;

    var password = document.getElementById('register-password').value;
    var confirmPassword = document.getElementById('register-confirm-password').value;


    if (email == "" || firstName == "" || lastName == "" || password == "" || confirmPassword == ""){
        document.getElementById('register-email').setAttribute("class", "input is-danger");
        document.getElementById('register-first').setAttribute("class", "input is-danger");
        document.getElementById('register-last').setAttribute("class", "input is-danger");
        document.getElementById('register-password').setAttribute("class", "input is-danger");
        document.getElementById('register-confirm-password').setAttribute("class", "input is-danger");
        document.getElementById("badlogin").style.display = "inline";
    }
    else if (!document.getElementById('terms-accept').checked) {
        document.getElementById("badterms").style.display = "inline";

    }
    else if (password != confirmPassword) {
        document.getElementById('register-password').setAttribute("class", "input is-danger");
        document.getElementById('register-confirm-password').setAttribute("class", "input is-danger");
        document.getElementById("badpass").style.display = "inline";

    }
    else {
    database.ref('/users/' + emailEncoded).once('value').then(function(snapshot){

            if (snapshot.val() != null) {
                document.getElementById('register-email').setAttribute("class", "input is-danger");
                document.getElementById("bademail").style.display = "inline";
                return;
            }
            else {
                database.ref('/users/' + emailEncoded).set({
                    email: email,
                    name: firstName + " " + lastName,
                    password: password
                })

                database.ref('/users/' + emailEncoded + '/preferences/').set({
                    dayStart: "09:00",
                    dayEnd : "17:00",
                    emailOptIn : false
    
                });

                document.cookie = "username=" + emailEncoded;
                document.cookie = "name=" + firstName + " " + lastName;
                document.location.href='tasklist.html'
            }

        })
    }
    
}

document.addEventListener("keyup", function(event) {
    // Number 13 is the "Enter" key on the keyboard
    if (event.keyCode === 13) {
      // Cancel the default action, if needed
      event.preventDefault();
      // Trigger the button element with a click
      document.getElementById("submitButton").click();
    }
  }); 
// function getUser() {
//     return window.localStorage.getItem("userID");
// }

// function getTaskPage() {
//     return 'task' + getUser() + '.html';
// }

// function getCalendarPage() {
//     return 'calendar' + getUser() + '.html';
// }

// function getSuggestionsPage() {
//     return 'suggestions' + getUser() + '.html';
// }

// function getProfilePage() {
//     return 'profile' + getUser() + '.html';
// }

// function getNavbarPage() {
//     return 'navbar' + getUser() + '.html';
// }