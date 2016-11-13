/* 
 * Script Javascript pour l'application BMI
 * corrigé SIMIER Philippe
 */

function conv(option){
    var foot,inch,cm;
    
    if (option == 1 || option == 2){
        foot = parseInt(document.getElementById('foot').value);
        inch = parseInt(document.getElementById('inch').value);
    
        cm = Math.round((foot*12 + inch)* 2.54);
    
        document.getElementById('cm').value = cm;
    }
    
    if (option == 3){
        cm = parseInt(document.getElementById('cm').value);
        foot = Math.round(cm / 30.48);
        inch = Math.round(cm /2.54)%12;
        console.log(foot);
        document.getElementById("foot").selectedIndex = foot-3;
        document.getElementById("inch").selectedIndex = inch;
    }
    
}

function unit(){
    choix = document.getElementById("units").value;
    poids = parseInt(document.getElementById("wg").value);
    if (choix == "pounds"){
        document.getElementById("wg").value = poids*2;
    } 
    if (choix == "kilograms"){
        document.getElementById("wg").value = poids/2;
    }
}

function calc(){
    taille = parseInt(document.getElementById('cm').value)/100;  // en metre
    
    choix = document.getElementById("units").value;
    poids = parseInt(document.getElementById("wg").value);
    if (choix == "pounds"){             // en kg
        poids = poids/2;
    }
     
     
    imc = poids/(taille*taille);
    // // arrondi 1 chiffre après la virgule
    document.getElementById('bmi').value = Math.round(imc*10)/10;  
    
     if (imc < 16.5){
        document.getElementById('corpulence').value = "Famine";
    }
    
    if (imc > 16.5 && imc < 18.5){
        document.getElementById('corpulence').value = "Maigreur";
    }
       
    if (imc > 18.5 && imc <= 25){
        document.getElementById('corpulence').value = "Normale";
    }
   
    if (imc > 25 && imc <= 30){
        document.getElementById('corpulence').value = "Surpoids";
    }
    
    if (imc > 30 && imc < 35){
        document.getElementById('corpulence').value = "Obésité modérée";
    }
    
    if (imc > 35 && imc < 40){
        document.getElementById('corpulence').value = "Obésité sévère";
    }
    
    if (imc > 40){
        document.getElementById('corpulence').value = "Obésité morbide";
    }
}




