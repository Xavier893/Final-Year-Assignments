        program SimpleStats
            implicit none
            integer :: num_elements
            integer, allocatable :: arr(:)
            integer :: smallest, largest, mode_val

            ! Interface block for readData subroutine
            interface
                subroutine readData(arr, num_elements)
                    implicit none
                    integer, intent(out), allocatable :: arr(:)
                    integer, intent(in) :: num_elements
                end subroutine readData
            end interface

            ! Interface block for findSmallest subroutine
            interface
                subroutine findSmallest(arr, smallest)
                    implicit none
                    integer, intent(in) :: arr(:)
                    integer, intent(out) :: smallest
                end subroutine findSmallest
            end interface

            ! Interface block for findLargest subroutine
            interface
                subroutine findLargest(arr, largest)
                    implicit none
                    integer, intent(in) :: arr(:)
                    integer, intent(out) :: largest
                end subroutine findLargest
            end interface

            ! Interface block for mode subroutine
            interface
                subroutine mode(arr, mode_val)
                    implicit none
                    integer, intent(in) :: arr(:)
                    integer, intent(out) :: mode_val
                end subroutine mode
            end interface

            ! Prompt the user to enter the number of elements
            print *, "Enter the number of elements:"
            read *, num_elements

            ! Allocate the array based on user input size
            allocate(arr(num_elements))

            ! Call the readData subroutine
           call readData(arr, num_elements)

            ! Find the smallest and largest values
            call findSmallest(arr, smallest)
            call findLargest(arr, largest)

            ! Find the mode
            call mode(arr, mode_val)

            ! Print the results
            print *, "Smallest value:", smallest
            print *, "Largest value:", largest
            print *, "Mode value:", mode_val

            ! Deallocate the array
           deallocate(arr)
        end program SimpleStats

        ! Subprogram to read data into the array
        subroutine readData(arr, num_elements)
            implicit none
            integer, intent(out), allocatable :: arr(:)
            integer, intent(in) :: num_elements
            integer :: i

            allocate(arr(num_elements)) ! Allocate the array based on the input size

            do i = 1, num_elements
                print *, "Enter value ", i, ":"
                read *, arr(i)
            end do
        end subroutine readData

        ! Subprogram to find the smallest value in the array
        subroutine findSmallest(arr, smallest)
            implicit none
            integer, intent(in) :: arr(:)
            integer, intent(out) :: smallest
            integer :: i

            smallest = arr(1)
            do i = 2, size(arr)
                if (arr(i) < smallest) then
                    smallest = arr(i)
                end if
            end do
        end subroutine findSmallest

        ! Subprogram to find the largest value in the array
        subroutine findLargest(arr, largest)
            implicit none
            integer, intent(in) :: arr(:)
            integer, intent(out) :: largest
            integer :: i

            largest = arr(1)
            do i = 2, size(arr)
                if (arr(i) > largest) then
                    largest = arr(i)
                end if
            end do
        end subroutine findLargest

        ! Subprogram to find the mode of the array
        subroutine mode(arr, mode_val)
            implicit none
            integer, intent(in) :: arr(:)
            integer, intent(out) :: mode_val
            integer :: i, j, count, max_count, candidate

            max_count = 0
            do i = 1, size(arr)
                count = 0
                do j = 1, size(arr)
                    if (arr(j) == arr(i)) then
                        count = count + 1
                    end if
                end do
                if (count > max_count) then
                    max_count = count
                    candidate = arr(i)
                end if
            end do
            mode_val = candidate
        end subroutine mode
