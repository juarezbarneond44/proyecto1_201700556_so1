package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"time"

	"github.com/gorilla/mux"
	"github.com/shirou/gopsutil/cpu"
	"github.com/shirou/gopsutil/mem"
)

type Ram struct {
	Total      string `json:"total"`
	Porcentaje string `json:"porcentaje"`
	Usado      string `json:"usado"`
}

type Cpu struct {
	Nucleo1 string `json:"nucleo1"`
	Libre1  string `json:"libre1"`
}
type Proceso struct {
	Procesos InfoCpu `json:"procesos"`
}
type InfoCpu struct {
	Pid    int         `json:"pid"`
	Nombre string      `json:"nombre"`
	Estado int         `json:"estado"`
	Hijo   InfoCpuHijo `json:"hijo"`
}
type InfoCpuHijo struct {
	Pid    int    `json:"pid"`
	Nombre string `json:"nombre"`
	Estado int    `json:"estado"`
}

//funcion que realizara los datos de la ram
func homeRAM(w http.ResponseWriter, req *http.Request) {
	enableCors(&w) // habilitamos cors
	//var Datoram Ram

	println("******** Cargar Datos RAM******")
	println("201700556")

	// funcion para obtener datos de la ram
	v, _ := mem.VirtualMemory()
	var Total = v.Total / (1024 * 1024)                  //valor total de ram en mb
	var Porcentaje = v.UsedPercent                       // porcentaje de utiliacion
	var used = uint64(float64(Total) * Porcentaje / 100) // cantidad de memoria utilizada

	datos := Ram{
		Total:      fmt.Sprintf("%v", Total),
		Porcentaje: fmt.Sprintf("%f", Porcentaje),
		Usado:      fmt.Sprintf("%v", used),
	}

	b, err := json.MarshalIndent(datos, "", "  ")
	if err != nil {
		fmt.Println(err)
	}
	// valores en consola
	fmt.Print(string(b))

	// enviamos en formato json los datos de la ram
	json.NewEncoder(w).Encode(datos)

}
func homeCPU(w http.ResponseWriter, r *http.Request) {
	enableCors(&w)
	println("******** Cargar Datos CPU******")
	println("201700556")
	//w.Write([]byte("Datos Del CPU")

	percent, _ := cpu.Percent(time.Second, true)
	var nucleo1 = percent[0]
	var libre1 = 100 - nucleo1

	fmt.Printf("nucleo 1: %v\n", nucleo1)
	fmt.Printf("libre 1: %v,\n", libre1)
	dato := Cpu{
		Nucleo1: fmt.Sprintf("%v", nucleo1),
		Libre1:  fmt.Sprintf("%v", libre1),
	}
	b, err := json.MarshalIndent(dato, "", "  ")
	if err != nil {
		fmt.Println(err)
	}
	// valores en consola
	fmt.Print(string(b))
	json.NewEncoder(w).Encode(dato)
}
func datosCPU(w http.ResponseWriter, r *http.Request) {
	enableCors(&w)
	println("******** Cargar Datos CPU******")
	println("201700556")
	b, err := ioutil.ReadFile("/proc/cpu_201700556") // obtenemos el archivo
	if err != nil {
		fmt.Print(err)
	}
	str := string(b) // convert content to a 'string'

	//fmt.Println(str) // print the content as a 'string'

	json.NewEncoder(w).Encode(str)
}

func main() {
	router := mux.NewRouter()
	router.HandleFunc("/Ram", homeRAM).Methods("GET")
	router.HandleFunc("/DatoCpu", datosCPU).Methods("GET")
	router.HandleFunc("/Cpu", homeCPU).Methods("GET")
	// levantamos el servidor en el puerto 4444 xD
	log.Fatal(http.ListenAndServe(":4444", router))
}

// esta funcion sirve para poder mandar peticiones a angular
func enableCors(w *http.ResponseWriter) {
	(*w).Header().Set("Access-Control-Allow-Origin", "*")
}
