#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/kernel_stat.h>
#include <linux/seq_file.h>

#define DEBUGFS_DIR "cpustat"
static struct dentry *cpustat_dir;

// Функция вывода информации о cpustat
static int cpustat_show(struct seq_file *m, void *v) {
    int i;
    for_each_online_cpu(i) {
        seq_printf(m, "CPU %d:\n", i);
        seq_printf(m, "  User time: %llu\n", kcpustat_cpu(i).cpustat[CPUTIME_USER]);
        seq_printf(m, "  System time: %llu\n", kcpustat_cpu(i).cpustat[CPUTIME_SYSTEM]);
        seq_printf(m, "  Idle time: %llu\n", kcpustat_cpu(i).cpustat[CPUTIME_IDLE]);
        seq_printf(m, "  IRQ count: %lu\n", kstat_cpu_irqs_sum(i));
    }
    return 0;
}

static int cpustat_open(struct inode *inode, struct file *file) {
    return single_open(file, cpustat_show, NULL);
}

// Описание операций для файла debugfs
static const struct file_operations cpustat_fops = {
    .owner = THIS_MODULE,
    .open = cpustat_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

// Стартовая функция модуля
int init_module(void) {
    // Создаём файл в debugfs
    cpustat_dir = debugfs_create_file(DEBUGFS_DIR, 0444, NULL, NULL, &cpustat_fops);
    if (!cpustat_dir) {
        pr_err("Failed to create debugfs file\n");
        return -ENOMEM;
    }
    pr_info("cpustat module loaded\n");
    return 0;
}

// Завершающая функция модуля
void cleanup_module(void) {
    // Удаляем файл из debugfs
    debugfs_remove(cpustat_dir);
    pr_info("cpustat module unloaded\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("marakkk");
MODULE_DESCRIPTION("module for lab 3 osi");

