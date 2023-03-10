package com.cloudnotes.service.controller;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.Catalogue;
import com.cloudnotes.service.service.CatalogService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping(value = "/catalog")
@CrossOrigin
public class CatalogController {
    @Autowired
    CatalogService catalogService;

    @GetMapping("/find")
    public List<Catalogue> find(Integer userId) {
        return catalogService.find(userId);
    }
    @PostMapping("/update")
    public Result update(@RequestBody Catalogue catalogue) {
        return catalogService.update(catalogue);
    }
    @PostMapping("/create")
    public Result create(@RequestBody Catalogue catalogue){
        return catalogService.create(catalogue);
    }
    @GetMapping("/delete")
    public Result delete(Integer id) {
        return catalogService.delete(id);
    }
}
