/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

static pd_entry_t *kpd = (pd_entry_t *)KERNEL_PAGE_DIR;
static pt_entry_t *kpt = (pt_entry_t *)KERNEL_PAGE_TABLE_0;

static const uint32_t identity_mapping_end = 0x003FFFFF;
static const uint32_t user_memory_pool_end = 0x02FFFFFF;

static paddr_t next_free_kernel_page = 0x100000;
static paddr_t next_free_user_page = 0x400000;

/**
 * kmemset asigna el valor c a un rango de memoria interpretado
 * como un rango de bytes de largo n que comienza en s
 * @param s es el puntero al comienzo del rango de memoria
 * @param c es el valor a asignar en cada byte de s[0..n-1]
 * @param n es el tamaño en bytes a asignar
 * @return devuelve el puntero al rango modificado (alias de s)
 */
static inline void *kmemset(void *s, int c, size_t n)
{
  uint8_t *dst = (uint8_t *)s;
  for (size_t i = 0; i < n; i++)
  {
    dst[i] = c;
  }
  return dst;
}

/**
 * zero_page limpia el contenido de una página que comienza en addr
 * @param addr es la dirección del comienzo de la página a limpiar
 */
static inline void zero_page(paddr_t addr)
{
  kmemset((void *)addr, 0x00, PAGE_SIZE);
}

void mmu_init(void)
{
}

/**
 * mmu_next_free_kernel_page devuelve la dirección de la próxima página de kernel disponible
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de kernel
 */
paddr_t mmu_next_free_kernel_page(void)
{
  if (next_free_kernel_page == identity_mapping_end)
  {
    kassert(0, "JAJA NO MEMORIA LOL");
  }
  int32_t a = next_free_kernel_page;
  next_free_kernel_page += PAGE_SIZE; // dejo la prox pagina libre
  return a;
}

/**
 * mmu_next_free_user_page devuelve la dirección de la próxima página de usuarix disponible
 * @return devuelve la dirección de memoria de comienzo de la próxima página libre de usuarix
 */
paddr_t mmu_next_free_user_page(void)
{
  if (next_free_user_page == user_memory_pool_end)
  {
    kassert(0, "JAJA NO MEMORIA LOL");
  }
  int32_t a = next_free_user_page;
  next_free_user_page += PAGE_SIZE; // dejo la prox pagina libre
  return a;
}

/**
 * mmu_init_kernel_dir inicializa las estructuras de paginación vinculadas al kernel y
 * realiza el identity mapping
 * @return devuelve la dirección de memoria de la página donde se encuentra el directorio
 * de páginas usado por el kernel
 */
paddr_t mmu_init_kernel_dir(void)
{
  kpd[0].attrs = 3; // = MMU_P | MMU_W , PRESENT Y WRITE
  kpd[0].pt = KERNEL_PAGE_TABLE_0 >> 12;

  for (int i = 0; i < 1024; i++)
  {
    kpt[i].attrs = 3;
    kpt[i].page = i;
  }

  return KERNEL_PAGE_DIR;
}

/**
 * mmu_map_page agrega las entradas necesarias a las estructuras de paginación de modo de que
 * la dirección virtual virt se traduzca en la dirección física phy con los atributos definidos en attrs
 * @param cr3 el contenido que se ha de cargar en un registro CR3 al realizar la traducción
 * @param virt la dirección virtual que se ha de traducir en phy
 * @param phy la dirección física que debe ser accedida (dirección de destino)
 * @param attrs los atributos a asignar en la entrada de la tabla de páginas
 */
void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs)
{
  uint32_t directory = virt >> 22;
  uint32_t table = (virt >> 12) & 0x003FF;

  // uint32_t PageBase = phy >> 12;
  pd_entry_t *PD = (pd_entry_t *)(CR3_TO_PAGE_DIR(cr3));
  pd_entry_t *PDE = &PD[directory]; // hay que definir un putero A la entrada para modificarla
  // pt_entry_t* PTbase = 0;

  if ((PDE->attrs & MMU_P) == 0)
  { // si no está presente lo definimos
    paddr_t new = mmu_next_free_kernel_page();
    zero_page(new);
    PDE->pt = new >> 12;
    PDE->attrs |= MMU_P;
  }
  // PTbase = (pt_entry_t*)(PDE.pt << 12);
  PDE->attrs |= (attrs | MMU_W | MMU_U);
  pt_entry_t *PTE = &((pt_entry_t *)(PDE->pt << 12))[table];
  PTE->page = phy >> 12;
  // PTE.attrs = 0;
  PTE->attrs |= (attrs | MMU_P);
  tlbflush();
}

/**
 * mmu_unmap_page elimina la entrada vinculada a la dirección virt en la tabla de páginas correspondiente
 * @param virt la dirección virtual que se ha de desvincular
 * @return la dirección física de la página desvinculada
 */
paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt)
{
  uint32_t directory = virt >> 22;
  uint32_t table = (virt >> 12) & 0x003FF;

  pd_entry_t *PDE = &((pd_entry_t *)(CR3_TO_PAGE_DIR(cr3)))[directory];
  if ((PDE->attrs & 0x1) == 1)
  {
    pt_entry_t *PTE = &((pt_entry_t *)(PDE->pt << 12))[table];
    PTE->attrs = 0;
    tlbflush();
    return (PTE->page << 12);
  }

  kassert(0, "NO ESTA MAPEADA");
  return 0;
}

#define DST_VIRT_PAGE 0xA00000
#define SRC_VIRT_PAGE 0xB00000

/**
 * copy_page copia el contenido de la página física localizada en la dirección src_addr a la página física ubicada en dst_addr
 * @param dst_addr la dirección a cuya página queremos copiar el contenido
 * @param src_addr la dirección de la página cuyo contenido queremos copiar
 *
 * Esta función mapea ambas páginas a las direcciones SRC_VIRT_PAGE y DST_VIRT_PAGE, respectivamente, realiza
 * la copia y luego desmapea las páginas. Usar la función rcr3 definida en i386.h para obtener el cr3 actual
 */
static inline void copy_page(paddr_t dst_addr, paddr_t src_addr)
{
  //breakpoint();
  //zero_page(dst_addr); --DA PAGE FAULT PORQUE NO ESTÁ MAPEADA, Y RECIBE COMO PARÁMETRO UNA VIRTUAL
  //breakpoint();
  uint32_t CR3 = CR3_TO_PAGE_DIR(rcr3()); //quiero leer el CR3 actual
  mmu_map_page(CR3, DST_VIRT_PAGE, dst_addr, 0x7);
  mmu_map_page(CR3, SRC_VIRT_PAGE, src_addr, 0x3);
  //breakpoint();
  uint8_t *dst = ((uint8_t *)(DST_VIRT_PAGE));
  uint8_t *src = ((uint8_t *)(SRC_VIRT_PAGE));
  for (uint32_t i = 0; i < PAGE_SIZE; i++)
  {

    dst[i] = src[i];
  }
  mmu_unmap_page(CR3, DST_VIRT_PAGE);
  mmu_unmap_page(CR3, SRC_VIRT_PAGE);
  //breakpoint();
}

/**
 * mmu_init_task_dir inicializa las estructuras de paginación vinculadas a una tarea cuyo código se encuentra en la dirección phy_start
 * @pararm phy_start es la dirección donde comienzan las dos páginas de código de la tarea asociada a esta llamada
 * @return el contenido que se ha de cargar en un registro CR3 para la tarea asociada a esta llamada
 */
paddr_t mmu_init_task_dir(paddr_t phy_start)
{
  uint32_t cr3 = mmu_next_free_kernel_page();
  zero_page(cr3);
  pd_entry_t  *PD = (pd_entry_t *)(CR3_TO_PAGE_DIR(cr3));
  PD[0].attrs = (MMU_P | MMU_W);
  PD[0].pt = (KERNEL_PAGE_TABLE_0 >> 12);
  uint32_t attrs = 0x5; // usuario present read-only
  mmu_map_page(cr3, TASK_CODE_VIRTUAL, phy_start, attrs);
  mmu_map_page(cr3, TASK_CODE_VIRTUAL + PAGE_SIZE, phy_start + PAGE_SIZE, attrs);
  attrs = 0x7; // usuario present read/write
  mmu_map_page(cr3, TASK_STACK_BASE, mmu_next_free_user_page(), attrs);
  attrs = 0x5;
  mmu_map_page(cr3, TASK_SHARED_PAGE, SHARED, attrs);
  return cr3;
}

// breakpoint()

void testxd(void)
{
  breakpoint();
  copy_page((paddr_t)0x500000, (paddr_t)0x000000);
  breakpoint();
}
