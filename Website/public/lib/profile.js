"use strict";

var config = {
    authDomain: "northwestern-task.firebaseapp.com",
    databaseURL: "https://northwestern-task.firebaseio.com"
};

if (!firebase.apps.length) {
    firebase.initializeApp(config);
}
var database = firebase.database();

var userID = document.cookie.replace(/(?:(?:^|.*;\s*)username\s*\=\s*([^;]*).*$)|^.*$/, "$1");
var user_name = document.cookie.replace(/(?:(?:^|.*;\s*)name\s*\=\s*([^;]*).*$)|^.*$/, "$1");

var preferencesURL = '/users/' + userID + "/preferences/";

var workStartEl = document.getElementById('work-start');
var workEndEl = document.getElementById('work-end');
var optInEl = document.getElementById('newsletter-input');

document.addEventListener('DOMContentLoaded', function () {

    database.ref(preferencesURL).once('value', function (snapshot) {
        if (snapshot.val() == null) {
            database.ref(preferencesURL).set({
                dayStart: "09:00",
                dayEnd: "17:00",
                emailOptIn: false

            });
        } else {
            workStartEl.value = snapshot.val().dayStart;
            workEndEl.value = snapshot.val().dayEnd;
            optInEl.checked = snapshot.val().emailOptIn;
        }
    });
});

function submitForm() {
    database.ref(preferencesURL).set({
        dayStart: workStartEl.value,
        dayEnd: workEndEl.value,
        emailOptIn: optInEl.checked
    });

    alert('Preferences saved!');
}

document.addEventListener('DOMContentLoaded', function () {
    var user_name = document.cookie.replace(/(?:(?:^|.*;\s*)name\s*\=\s*([^;]*).*$)|^.*$/, "$1");
    var user_id = document.cookie.replace(/(?:(?:^|.*;\s*)username\s*\=\s*([^;]*).*$)|^.*$/, "$1");
    if (user_name == "" || user_id == "") {
        alert("You have been logged out.");
        document.location.href = 'index.html';
    }
});