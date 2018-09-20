"# ControlUVEstanque" 
Este pequeño autómata gestiona una lampara UV para esterilización de agua de un estanque que se remonta por medio de bombas solares a un filtro de plantas de agua dulce.
La lámpara UV está ubicada en el circuido de bombeo de tal forma que al pasar el agua se esteriliza.
El sistema consiste en un detector de flujo de corriente (un sensor amperímetro) sobre la corriente que las placas solares suministran a las bombas. A partir de un umbral
de intensidad mínimo y durante un tiempo continuo establecido (1 min) activa un relé de 220V sobre la lámpara UV. Esta señal de la lámpara se mantiene mientras exista corriente
hasta que el umbral disminuya por un tiempo de 1 minuto.
