#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/fs.h>

struct task_struct *task; //estructura definida en sched.h para tareas/procesos
struct task_struct *childtask; //estructura necesaria para iterar a travez de procesos secundarios
struct task_struct *memtask; 
struct list_head *list; // estructura necesaria para recorrer cada lista de tareas tarea->estructura de hijos

static int escribir_archivo(struct seq_file * archivo, void *v){
for_each_process( task ){
seq_printf(archivo, "{\n");
	seq_printf(archivo, "\"pid\": %d,\n",task->pid);
	seq_printf(archivo, "\"nombre\": \"%s\",\n",task->comm);
	seq_printf(archivo, "\"usuario\": \"root\",\n");
	seq_printf(archivo, "\"estado\": %ld,\n",task->state);
	seq_printf(archivo, "\"hijo\":\n");
	seq_printf(archivo, "\t[\n");
		list_for_each( list,&task->children ){
	seq_printf(archivo, "\t{\n");
			childtask= list_entry( list, struct task_struct, sibling );
	//printk(KERN_INFO "HIJO DE %s[%d]PID: %d PROCESO: %s ESTADO: %ld",task->comm,task->pid,childtask->pid, childtask->comm, childtask->state);
	seq_printf(archivo, "\t\"pid\": %d,\n",childtask->pid);
	seq_printf(archivo, "\t\"nombre\": \"%s\",\n",childtask->comm);
	seq_printf(archivo, "\t\"usuario\": \"root\",\n");
	seq_printf(archivo, "\t\"estado\": %ld,\n",childtask->state);
	//seq_printf(archivo, "\"ram\": %d\n",childtask->ram);
	seq_printf(archivo, "\t},\n");

	}
	seq_printf(archivo, "\t]\n");
	seq_printf(archivo, "},\n");
 
		printk("*****************");	
	}



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


int iniciar(void){ //modulo de inicio
	proc_create("cpu_201700556", 0, NULL, &operaciones);
	printk(KERN_INFO "%s", "CARGANDO MODULO");
	printk(KERN_INFO "JUAN CARLOS JUAREZ 201700556");
	
	 
	return 0;
}

void salir(void){
	remove_proc_entry("cpu_201700556",NULL);
	printk(KERN_INFO "%s","REMOVIENDO MODULO");
	printk(KERN_INFO "Curso: sistemas operativos 1\n");
}

module_init(iniciar);
module_exit(salir);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("201700556");