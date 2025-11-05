from ovito.io import import_file
from ovito.vis import Viewport
import os

# ===== CONFIGURACIÓN =====
input_file = "output/posicionT.xyz"
output_dir = "frames"
image_size = (800, 600)
particle_radius = 0.3

# ===== CARGA DEL ARCHIVO =====
print(f"Cargando archivo: {input_file}")
pipeline = import_file(input_file)

num_frames = pipeline.source.num_frames
print(f"Frames detectados: {num_frames}")

# ===== CONFIGURAR VISUALIZACIÓN =====
pipeline.add_to_scene()

particles_vis = pipeline.source.data.particles.vis
particles_vis.radius = particle_radius
# 🔹 No seteamos color, usa el default

# ===== CREAR CARPETA =====
os.makedirs(output_dir, exist_ok=True)

# ===== CONFIGURAR VIEWPORT =====
vp = Viewport()
vp.type = Viewport.Type.Perspective
vp.zoom_all()

# ===== RENDERIZAR FRAMES =====
for frame in range(num_frames):
    output_path = os.path.join(output_dir, f"frame_{frame:04d}.png")
    print(f"Renderizando frame {frame+1}/{num_frames} → {output_path}")
    vp.render_image(
        filename=output_path,
        size=image_size,
        background=(1, 1, 1),
        frame=frame
    )

print("\n✅ Renderización completa.")
print(f"Imágenes guardadas en: {os.path.abspath(output_dir)}")
