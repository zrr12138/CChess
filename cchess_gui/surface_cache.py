import pygame


def singleton(cls):
    instances = {}

    def wrapper(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]

    return wrapper


@singleton
class SurfaceCache:
    def __init__(self):
        # 初始化代
        self.filePath2surface = {}

    def load(self, file_path: str):
        if file_path in self.filePath2surface:
            return self.filePath2surface[file_path]
        surface = pygame.image.load(file_path)
        mask = pygame.mask.from_surface(surface)
        outline = mask.outline()
        pygame.draw.polygon(surface, (217, 175, 111, 0), outline, 7)
        self.filePath2surface[file_path] = surface
        return surface
