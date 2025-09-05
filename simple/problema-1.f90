! Introducción a la Simulación Computacional
! Edición: 2025
! Docentes: Joaquín Torres y Claudio Pastorino

program simple 


    use ziggurat
    implicit none
    logical :: es
    integer :: seed,i ,j,k
    real (kind=8) :: x(10),a(10,10),b(10,10)
    real (kind=8), allocatable  :: y(:),c(:,:)
    real (kind=8) :: A_numero_aleatorio(2)
    integer :: input
    real (kind=8), allocatable :: B_numero_aleatorio(:,:)
    CHARACTER(LEN=1) :: nl
    

![NO TOCAR] Inicializa generador de número random

    inquire(file='seed.dat',exist=es)
    if(es) then
        open(unit=10,file='seed.dat',status='old')
        read(10,*) seed
        close(10)
        print *,"  * Leyendo semilla de archivo seed.dat"
    else
        seed = 24583490
    end if

    call zigset(seed)
![FIN NO TOCAR]    

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!! INICIO DE PROGRAMA !!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    open(unit=10,file='input.dat',status='old') ! Leemos el archivo
        read(10,*) input
    close(10)


    open(unit=10, file='output.dat')
        
        write(10,*) 'X   ','Y   '
        do i=1,input
            A_numero_aleatorio(1)   = uni() !! Primer numero aleatorio
            A_numero_aleatorio(2)   = uni() !! Segundo numero aleatorio

            if(A_numero_aleatorio(1) < A_numero_aleatorio(2)) then
                print *, A_numero_aleatorio(1) !! Imprimimos por pantalla

                write(10,*) A_numero_aleatorio(1), A_numero_aleatorio(2)
            end if
        end do
    close(10)

    !-----------------------------------------------
    ! Punto numero D -------------------------------
    !-----------------------------------------------
    
    allocate(B_numero_aleatorio(input,2)); ! Instanciamos al array que guardará numeros aleatorios
    open(unit=10, file='output_D.dat')
    
        write(10,*) 'X   ','Y   '

        do i=1,input ! Iteramos
            
            B_numero_aleatorio(i,1) = uni()
            B_numero_aleatorio(i,2) = uni()
            if((B_numero_aleatorio(i,2) > 0.5) .and. (B_numero_aleatorio(i,1) < B_numero_aleatorio(i,2))) then
                write(10,*) B_numero_aleatorio(i,1), B_numero_aleatorio(i,2)
            end if
        end do
    close(10)



!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!! FIN DE PROGRAMA !!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

!! 
!! F:IN FIN edicion
!! 
![No TOCAR]
! Escribir la última semilla para continuar con la cadena de numeros aleatorios 

        open(unit=10,file='seed.dat',status='unknown')
        seed = shr3() 
         write(10,*) seed
        close(10)
![FIN no Tocar]        


end program simple
