const t_first = gsap.timeline({default : {ease : 'power2.out'}});

t_first.to(".top", {opacity: "1", duration : 1});
t_first.to(".bottom", {opacity: "1", duration : 1}, "-=1");