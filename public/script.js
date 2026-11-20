function showPage(pageName) {

    let pages = document.getElementsByClassName("page");

    for (let i = 0; i < pages.length; i++) {
        pages[i].classList.add("hidden");
    }

    document.getElementById(pageName).classList.remove("hidden");
}

function showMessage(operation) {

    alert(operation + " form is ready. C++ connection will be added next.");
}

function exitProgram() {
    alert("You can now close this website.");
}
