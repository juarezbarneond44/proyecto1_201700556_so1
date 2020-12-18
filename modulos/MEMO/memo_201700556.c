#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#define  BUFSIZE 150
MODULE_LICENSE("GPL");
MODULE_AUTHOR("201700556");
struct sysinfo inf;
static int escribir_archivo(struct seq_file * archivo, void *v){
    si_meminfo(&inf);
    long total_memoria =(inf.totalram * 4);
    long memoria_libre =(inf.freeram*4);
	seq_printf(archivo, "****************\n");
	seq_printf(archivo, "*LABORATORIO DE SISTEMAS OPERATIVOS 1      *\n");
	seq_printf(archivo, "*DICIEMBRE 2020                            *\n");
	seq_printf(archivo, "*JUAN CARLOS JUAREZ BARNEOND 201700556     *\n");
	seq_printf(archivo, "*PROYECTO 1 MODULO DE LA RAM               *\n");
	seq_printf(archivo, "****************\n");
    seq_printf(archivo, "Memoria Total : \t %8lu MB\n",total_memoria/1024);
    seq_printf(archivo, "Memoria libre : \t %8lu MB\n",memoria_libre/1024);
    seq_printf(archivo, "Memoria en uso : \t %8lu MB\n",(total_memoria-memoria_libre)/1024);
    seq_printf(archivo, "Porcentaje de Memoria : \t %8lu \n",((total_memoria-memoria_libre)/total_memoria*100));
	return 0;
}

static int al_abrir(struct inode *inode, struct file*file){
	return single_open(file, escribir_archivo, NULL);	
}

static struct file_operations operaciones =
{
	.open = al_abrir,
	.read = seq_read
};


static int iniciar(void){ //modulo de inicio
	proc_create("memo_201700556", 0, NULL, &operaciones);
	printk(KERN_INFO "%s", "CARGANDO MODULO");
	printk(KERN_INFO "JUAN CARLOS JUAREZ 201700556");
 	return 0;
}

static void salir(void){
	remove_proc_entry("memo_201700556",NULL);
	printk(KERN_INFO "%s","REMOVIENDO MODULO");
	printk(KERN_INFO "Curso: sistemas operativos 1\n");
}

module_init(iniciar);
module_exit(salir);

 