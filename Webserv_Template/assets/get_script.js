const tl = gsap.timeline({default : {ease : 'power2.out'}});

tl.to(".header", {opacity: 1, duration: 1});

var input_ = document.querySelector('.input_part input')

input_.addEventListener('keydown', function(e) {
    if (e.key == "Enter") {
        var validValue = input_.value;
        input_.value = "";
        window.open(validValue, "_blank").focus();
    }
});