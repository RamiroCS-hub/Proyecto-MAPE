// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
// https://firebase.google.com/docs/web/setup#available-libraries
import {set, getDatabase, ref, get} from "firebase"
// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyDbl-00Gqp-54bl41O6hqTx83HyPh4hlyE",
  authDomain: "fir-test-50790.firebaseapp.com",
  databaseURL: "https://fir-test-50790-default-rtdb.firebaseio.com",
  projectId: "fir-test-50790",
  storageBucket: "fir-test-50790.appspot.com",
  messagingSenderId: "103354347929",
  appId: "1:103354347929:web:7c296fd5afc35b2dc16308",
  measurementId: "G-68G3HGY449"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
// const analytics = getAnalytics(app);

const db = getDatabase();

let name = document.querySelector("formBody__name")
let email = document.querySelector(".formBody__inp");
let btn = document.querySelector(".formBody__btn")

const insertData = () => {
    // set(ref(db,"\User"),{
    //     name:name.value,
    //     email:email.value
    // }).then(() =>{
    //     console.log("Data guardada correctamente");
    // }).catch((e) =>{
    //     console.log("El error fue",e);
    // })
    get(ref(db,"User/Alarma")
        ).then((snapshot) =>{
            console.log(snapshot)


    })
}

btn.addEventListener("click",insertData);

